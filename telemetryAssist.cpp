/*
This is an telemetry program for Luogu Submitting Crawler
Telemetry Assist Version: 1 Date: 2025/7/22
*/
#include<bits/stdc++.h>
#include<windows.h>
#define MAX_OPTION 2000
using namespace std;
const int MD5_BLOCK_SIZE=64,MD5_DIGEST_SIZE=16;
string uuid,guid,loCode,loNum,naCode,plat;
wchar_t value[256];
unsigned char teleID[MD5_DIGEST_SIZE]={};
char option[MAX_OPTION];
static const unsigned int T[64]={
    0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
    0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
    0x1fa27cf8,0x3f6a0b68,0x9c1e3a60,0x1111f4e2,0xd6d6f5d1,0x75f7c8b8,0xa3c4e6b2,0x1c6ec54f,
    0x349bc0fc,0x7f3c4201,0x47fd7b9a,0x9c0f50ab,0x8c5c2a24,0xe639a194,0x7bc7103a,0x08afce79,
    0x0bfa5a97,0x5a91a5a3,0x5368f8a4,0x7c9057cf,0xd3e8b0e3,0x4c09dfad,0x8d6b3be3,0x2b0ccab6,
    0x383f1a2d,0x6342347f,0x1d1285b7,0x49563c7f,0x17e8b276,0x835e8d07,0xa5f96ba7,0x43d98b6e,
    0x5b53ec60,0x9fa6bba0,0x9378300f,0xc1c7df53,0x6aab3b12,0x19d080b9,0x027bbdd9,0x09ad4b3e
};
inline unsigned int leftRotate(unsigned int x,int n){return (x<<n)|(x>>(32-n));}
void padMessage(const unsigned char* message,unsigned long long messageLength,unsigned char* paddedMessage,unsigned long long& paddedMessageLength){
    unsigned long long originalLength=messageLength*8,paddingLength=0;
    paddingLength=(56-messageLength%64)%64;paddedMessageLength=messageLength+paddingLength+8;
    memcpy(paddedMessage,message,messageLength);paddedMessage[messageLength]=0x80; 
    memset(paddedMessage+messageLength+1,0,paddingLength);
    memcpy(paddedMessage+paddedMessageLength-8,&originalLength,8);
}
void computeMD5(const unsigned char* message,unsigned long long messageLength,unsigned char* hash){
    unsigned int A=0x67452301,B=0xefcdab89,C=0x98badcfe,D=0x10325476;
    unsigned char paddedMessage[MD5_BLOCK_SIZE*64];unsigned long long paddedMessageLength;
    padMessage(message,messageLength,paddedMessage,paddedMessageLength);
    unsigned int* M=reinterpret_cast<unsigned int*>(paddedMessage);
    unsigned int tempA,tempB,tempC,tempD;
    for (unsigned long long i=0;i<paddedMessageLength/MD5_BLOCK_SIZE;i++) {
        tempA=A;tempB=B;tempC=C;tempD=D;
        for (int j=0;j<64;j++) {
            unsigned int F,g;
            if (j<16){F=(B&C)|((~B)&D);g=j;} 
			else if(j<32){F=(D&B)|((~D)&C);g=(5*j+1)%16;} 
			else if(j<48){F=B^C^D;g=(3*j+5)%16;}
			else {F=C^(B|(~D));g=(7*j)%16;}
            unsigned int temp=D;
            D=C;C=B;B=B+leftRotate((A+F+T[j]+M[g]),(j<16?7:(j<32?5:(j<48?4:6))));
            A=temp;
        }
        A+=tempA;B+=tempB;C+=tempC;D+=tempD;
    }
    memcpy(hash,&A,4);memcpy(hash+4,&B,4);memcpy(hash+8,&C,4);memcpy(hash+12,&D,4);
}
string toHexString(unsigned char* hash) {
    stringstream ss;
	for(int i=0;i<MD5_DIGEST_SIZE;i++){ss<<hex<<setw(2)<<setfill('0')<<(int)hash[i];}
    return ss.str();
}
void Exit(bool ret){
	bool res;
	do{
		if(ret){
			time_t nowtime;time(&nowtime);tm* p=localtime(&nowtime);
			sprintf(option,"echo %04d/%02d/%02d %02d:%02d:%02d > telemetryTime.txt",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
			res=system(option);			
			sprintf(option,"echo %.2lf >> telemetryTime.txt",(double)clock()/CLOCKS_PER_SEC);
			res|=system(option);res|=system("echo Success > telemetryStatus.txt");
		}
		else res=system("echo Fail > telemetryStatus.txt");
	}while(res);
	exit(0);
}
void read_info(){
	ifstream fin;ofstream fout;
	fin.open("sysinfo.txt");if(!fin.is_open())Exit(0);
	getline(fin,uuid);getline(fin,guid);uuid+=guid;
    computeMD5(reinterpret_cast<const unsigned char*>(uuid.c_str()),uuid.size(),teleID);
	for(int i=1;i<=3;i++)loCode="",getline(fin,loCode);
	getline(fin,loNum);getline(fin,naCode);fin.close();
	system("chcp 65001 & systeminfo > telemetryTmp.txt");	
	fin.open("telemetryTmp.txt",ios::in);
	if(!fin.is_open())Exit(0);
	while(true){
		string tmp="",key="";
		int i;bool flg=0;getline(fin,tmp);
		for(i=0;tmp[i]!=':'&&i<tmp.length();i++)key+=tmp[i];
		if(key!="OS Name")continue;fin.close();
		for(;i<tmp.length();i++){if(isalpha(tmp[i]))flg=1;if(flg)plat+=tmp[i];}
		break;
	}
	HKEY hKey;DWORD value_length=sizeof(value);
    const wchar_t* subKey=L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
    const wchar_t* valueName=L"DisplayVersion";
    LONG result=RegOpenKeyExW(HKEY_LOCAL_MACHINE,subKey,0,KEY_READ|KEY_WOW64_64KEY,&hKey);
    if(result!=ERROR_SUCCESS)Exit(0);
    result=RegQueryValueExW(hKey,valueName,nullptr,nullptr,(LPBYTE)value,&value_length);
    if(result!=ERROR_SUCCESS)Exit(0);;RegCloseKey(hKey);
}
void make_file(){
	string fileName;
	ifstream fin;ofstream fout;wofstream wfout;
	fin.open("telemetry.txt",ios::in);if(!fin.is_open())Exit(0);
	fout.open("email.txt",ios::out);
	fout<<"From: rcaa_receiver@sina.com"<<endl;
	fout<<"To: rcaa_receiver@sina.com"<<endl;
	fout<<"Subject: LSC Telemetry Data"<<endl;
	fout<<"MIME-Version: 1.0"<<endl;
	fout<<"Content-Type: multipart/mixed; boundary=\"===BOUNDARY===\""<<endl<<endl;
	fout<<"--===BOUNDARY==="<<endl;
	fout<<"Content-Type: text/plain; charset=\"utf-8\""<<endl;
	fout<<"Content-Transfer-Encoding: 7bit"<<endl<<endl;
	fout<<"Platform: "<<plat<<" ";fout.close();
	wfout.open("email.txt",ios::app);wfout<<value<<endl;wfout.close();
	fout.open("email.txt",ios::app);fout<<"Telemetry ID: "<<toHexString(teleID)<<endl;
	fout<<"Nation & Region: "<<naCode<<" ("<<loCode<<","<<loNum<<")"<<endl<<endl;
	fout.close();
	while(true){
		fileName="";getline(fin,fileName);
		if(fileName.empty()){fin.close();break;}
		fout.open("email.txt",ios::app);fout<<endl<<"--===BOUNDARY==="<<endl;
		fout<<"Content-Type: text/plain; charset=\"utf-8\""<<endl;
		fout<<"Content-Transfer-Encoding: base64"<<endl;
		fout<<"Content-Disposition: attachment; filename=\""<<fileName<<"\""<<endl<<endl;fout.close();
		sprintf(option,"powershell -Command \"[Convert]::ToBase64String([IO.File]::ReadAllBytes('%s'))\" >> email.txt",fileName.c_str());
		system(option);
	}
	fout.open("email.txt",ios::app);fout<<endl<<"--===BOUNDARY===--";fout.close();
}
void send(){
	/*Code was removed*/
}
void start(){
	bool res;
	do{res=system("echo Running > telemetryStatus.txt");}while(res);
	system("del telemetryTime > nul 2>nul");
}
int main(){
	start();
	read_info();
	make_file();
	send();
	return 0;
}