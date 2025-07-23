/*
This is a program to get system information for Luogu Submitting Crawler
System Getor Version: 2 Date: 2025/7/22
*/
#include<Windows.h>
#include<windows.h>
#include<string>
#include<tchar.h>
#include<bits/stdc++.h>
#include<iostream>
using namespace std;
typedef struct _dmi_header{BYTE type;BYTE length;WORD handle;}dmi_header;
typedef struct _RawSMBIOSData{BYTE Used20CallingMethod;BYTE SMBIOSMajorVersion;BYTE SMBIOSMinorVersion;BYTE DmiRevision;DWORD Length;BYTE SMBIOSTableData[];}RawSMBIOSData;
static void dmi_system_uuid(const BYTE *p, short ver){
    int only0xFF=1,only0x00=1,i;
    for(i=0;i<16&&(only0x00||only0xFF);i++){if(p[i]!=0x00)only0x00=0;if(p[i]!=0xFF)only0xFF=0;}
    if(only0xFF){printf("Not Present");return;}if(only0x00){printf("Not Settable");return;}
    if(ver>=0x0206)
        printf("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
            p[3], p[2], p[1], p[0], p[5], p[4], p[7], p[6],
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
    else
        printf("-%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
}
const char *dmi_string(const dmi_header *dm,BYTE s){
    char *bp=(char *)dm;size_t i,len;
    if(s==0)return "Not Specified";
    bp += dm->length;
    while(s>1&&*bp){bp+=strlen(bp);bp++;s--;}
    if (!*bp)return "BAD_INDEX";
    /* ASCII filtering */
    len=strlen(bp);
    for(i=0;i<len;i++)if(bp[i]<32||bp[i]==127)bp[i]='.';
    return bp;
}
void getMachineGUID(){
	ifstream fin;ofstream fout;
    string name="MachineGuid";HKEY hKey;DWORD dwType=REG_SZ;DWORD dwLen=MAX_PATH;
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography",
        0,KEY_READ|KEY_WOW64_64KEY,&hKey)==ERROR_SUCCESS)cout<<"ok\n";
    unsigned char buf[100];PLONG data=0;
    if(RegQueryValueExA(hKey,name.c_str(),0,&dwType,(LPBYTE)buf,&dwLen)==ERROR_SUCCESS)cout<<"ok\n";
    else cout<<"GetLastError()="<< GetLastError()<<endl;
    string qstr="";char* p=(char*)buf;qstr=qstr.append(p);
    transform(qstr.begin(),qstr.end(),qstr.begin(),::toupper);
	fout.open("sysinfo.txt",ios::app);
    fout<<qstr<<endl;
	fout.close();
    cout<<"GUID "<<qstr<<endl;
}
void getName(){
	ifstream fin;ofstream fout;
	const int MAX_BUFFER_LEN=500;
	char szBuffer[MAX_BUFFER_LEN];
	DWORD dwNameLen;dwNameLen=MAX_BUFFER_LEN;dwNameLen=MAX_BUFFER_LEN;
	if (!GetComputerName(szBuffer,&dwNameLen))printf("Error  %d\n", GetLastError());
	fout.open("sysinfo.txt",ios::app);
	fout<<szBuffer<<endl;
	fout.close();
	
	memset(szBuffer,0,sizeof szBuffer);
	if (!GetUserName(szBuffer,&dwNameLen))printf("Error  %d\n", GetLastError());
	fout.open("sysinfo.txt",ios::app);
	fout<<szBuffer<<endl;
	fout.close();
}
int _tmain(int argc,_TCHAR* argv[]){
	freopen("log.txt","a",stdout);
	ifstream fin;ofstream fout;
	system("chcp 65001 > nul");SetConsoleOutputCP(65001);
	
    DWORD bufsize=0;BYTE buf[65536]={0};
    int ret=0,flag=1;RawSMBIOSData *Smbios;dmi_header *h=NULL;
    ret=GetSystemFirmwareTable('RSMB',0,0,0);
    if(!ret){printf("Function failed!\n");return 1;}
    printf("get buffer size is %d\n",ret);bufsize=ret;
    ret=GetSystemFirmwareTable('RSMB',0,buf,bufsize);
    if(!ret){printf("Function failed!\n");return 1;}
    Smbios=(RawSMBIOSData *)buf;
    BYTE *p=Smbios->SMBIOSTableData;
    if(Smbios->Length!=bufsize-8){printf("Smbios length error\n");return 1;}
    for(int i=0;i<Smbios->Length;i++){
        h=(dmi_header*)p;
        if(h->type==0&&flag){
            printf("\nType %02d - [BIOS]\n",h->type);
            printf("\tBIOS Vendor : %s\n", dmi_string(h,p[0x4]));
            printf("\tBIOS Version: %s\n", dmi_string(h,p[0x5]));
            printf("\tRelease Date: %s\n", dmi_string(h,p[0x8]));
            if(p[0x16]!=0xff&&p[0x17] != 0xff)printf("\tEC version: %d.%d\n", p[0x16],p[0x17]);
            flag=0;
        }else if(h->type==1){
            printf("\nType %02d - [System Information]\n", h->type);
            printf("\tManufacturer: %s\n", dmi_string(h, p[0x4]));
            printf("\tProduct Name: %s\n", dmi_string(h, p[0x5]));
            printf("\tVersion: %s\n", dmi_string(h, p[0x6]));
            printf("\tSerial Number: %s\n", dmi_string(h, p[0x7]));
            printf("\tUUID: ");dmi_system_uuid(p + 0x8, Smbios->SMBIOSMajorVersion * 0x100 + Smbios->SMBIOSMinorVersion);
			freopen("sysinfo.txt","w",stdout);
			dmi_system_uuid(p + 0x8, Smbios->SMBIOSMajorVersion * 0x100 + Smbios->SMBIOSMinorVersion);
            fclose(stdout);freopen("log.txt","a",stdout);
			printf("\tSKU Number: %s\n", dmi_string(h, p[0x19]));
            printf("\tFamily: %s\n", dmi_string(h, p[0x1a]));
        }
        p+=h->length;while((*(WORD *)p)!=0)p++;p+=2;
    }
    getMachineGUID();getName();
    system("localFetcher");
    return 0;
}