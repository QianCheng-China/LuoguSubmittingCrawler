#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<io.h>

#define prt printf
#define sprt sprintf
#define sys system

#define MAX_OPTION 2000
#define MAX_COOKIE 150
#define MAX_URL 100
#define MAX_HEAD 40

#define line prt("--------------\n")
#define enter putchar('\n')
#define space putchar(' ')
#define chos _getch()-'0'

using namespace std;
const string _DATE="2025.7.20";
const string _VERSION="2.0.1"; 
const int MD5_BLOCK_SIZE=64,MD5_DIGEST_SIZE=16;

string guid,uuid,pcName,acName,loCode,loNum,naCode,latestVer;

char option[MAX_OPTION];
int optRes,choice;

int saveOrder[5],delayTime,timeLimit;
bool saveAll,fileName[4];

string _uid,__client_id,decodeKey,userName;                 
char cookie[MAX_COOKIE],tarUrl[MAX_URL],curlHead[MAX_HEAD];
bool loginAble,readAble;

int tele_lev;//1=off 2=necessary 3=full
unsigned char teleID[MD5_DIGEST_SIZE]={};

bool night_work;
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

void telemetry(string file,int lev){
	if(lev>tele_lev)return;
	string file_sign="";
	int len=file.length();
	system("del telemetry.txt > nul");
	for(int i=0;i<=len;i++){
		if(file[i]==','||i==len){
			sprt(option,"echo %s >> telemetry.txt",file_sign.c_str());
			sys(option);file_sign="";	
		}else file_sign+=file[i]; 
	}
    ShellExecute(NULL,"open","telemetryAssist.exe",NULL,NULL,SW_HIDE);
}

void topbar(string s){
	sys("cls");prt("Luogu Submitting Crawler\n");
	loginAble?prt(""):prt("🔑");
	if(latestVer==_VERSION)prt("");
	else if(latestVer=="Fail")prt("⚠");
	else cout<<"🔄";
	tele_lev>1?prt("🛜"):prt("");
	if(loginAble&&latestVer==_VERSION&&tele_lev==1);
	else prt("|");prt("首页");
	if(s!=" ")prt("-"),cout<<s;enter;line;
}

void cookieCoder(bool decode){
	string a=guid,b=uuid;unsigned __int128 tmp=0;
	for(int i=0;guid[i];i++)tmp=tmp*100+(unsigned __int128)((guid[i] xor uuid[i] xor i)%100);
	for(int i=0;tmp!=0;i++)__client_id[i]+=(int)((tmp%10)*(decode?-1:1)),tmp/=10;
	if(decode)for(int i=0;__client_id[i];i++)if((!isdigit(__client_id[i]))&&(!isalpha(__client_id[i])))__client_id[i]='0';
}
bool saveSetting(){
	ifstream fin;ofstream fout;
	fout.open("setting.txt",ios::out);
	if(fout.is_open()){
		for(int i=1;i<=4;++i){fout<<saveOrder[i];i==4?fout<<endl:fout<<" ";}
		for(int i=1;i<=3;++i){fout<<fileName[i];i==3?fout<<endl:fout<<" ";}
		fout<<saveAll<<" "<<delayTime<<" "<<timeLimit<<" "<<tele_lev;	
		fout.close();return 1;
	}else return 0;
}

bool checkCookie(){
	sprt(tarUrl,"\"https://www.luogu.com.cn/\"");
	sprt(option,"%s -s %s %s > tmp.txt",curlHead,tarUrl,cookie);sys(option);
	int fn=0,bk=0,len;string html;bool matched=0;
	const string fSign="%22name%22%3A%22",bSign="%22%2C%22avatar%22%";
	ifstream fin;ofstream fout;
	fin.open("tmp.txt",ios::in);for(int i=1;i<=23;i++)html="",getline(fin,html);fin.close();
	bk=(len=html.length())-18;
	while(++fn<len-15){matched=1;for(int i=0;i<=15&&matched;i++)if(html[fn+i]!=fSign[i])matched=0;if(matched){fn+=16;break;}}
	if(!matched)return 0;
	while(--bk){matched=1;for(int i=0;i<=18&&matched;i++)if(html[bk+i]!=bSign[i])matched=0;if(matched){bk-=1;break;}}
	userName="";for(int i=fn;i<=bk;i++)userName+=html[i];
	return 1;
}

string timCov(time_t timeStamp){
    struct tm *timeinfo=NULL;char buffer[80];
    timeinfo=localtime(&timeStamp);
    strftime(buffer,80,"%Y-%m-%d %H-%M-%S",timeinfo);
    return string(buffer);
}
string numCov(int x){
	string ret="\0";
	while(x)ret+=(char)(x%10+'0'),x/=10;
	reverse(ret.begin(),ret.end());
	return ret;
}

string langID(int x){
	if(x==1)return ".pas";
	if(x==2)return ".c";
	if(x==28||x==3||x==4||x==11||x==17||x==27)return ".cpp";
	if(x==7||x==25||x==6||x==24)return ".py";
	if(x==8||x==33)return ".java";
	if(x==15)return ".rs";
	if(x==14)return ".go";
	if(x==19)return ".hs";
	if(x==16)return ".php";
	if(x==5)return ".ans";
	return ".txt";
}
unsigned char FromHex(unsigned char x) {
	unsigned char y;
	if (x>='A'&&x<='Z')y=x-'A'+10;
	else if(x>='a'&&x<='z')y=x-'a'+10;
	else if(x>='0'&&x<='9')y=x-'0';
	return y;
}
string urlDecode(const string& str) {
	string strTemp="";
	size_t length=str.length();
	for(size_t i=0;i<length;i++) {
		if(str[i]=='+')strTemp+=' ';
		else if(str[i]=='%'){
			unsigned char high=FromHex((unsigned char)str[++i]);
			unsigned char low=FromHex((unsigned char)str[++i]);
			strTemp+=high*16+low;
		}else strTemp+=str[i];
	}
	return strTemp;
}
string utfCov(const char* str) {
	string result;
	WCHAR *strSrc;
	LPSTR szRes;
	int i=MultiByteToWideChar(CP_UTF8,0,str,-1,NULL,0);
	strSrc=new WCHAR[i+1];
	MultiByteToWideChar(CP_UTF8,0,str,-1,strSrc,i);
	i=WideCharToMultiByte(CP_ACP,0,strSrc,-1,NULL,0,NULL,NULL);
	szRes=new CHAR[i+1];
	WideCharToMultiByte(CP_ACP,0,strSrc,-1,szRes,i,NULL,NULL);
	result=szRes;
	delete[] strSrc;delete[] szRes;
	return result;
}
string getProbName(string pidS,string cidS) {
	string probName="";
	if(cidS.empty())sprt(tarUrl,"\"https://www.luogu.com.cn/problem/%s\"",pidS.c_str());
	else sprt(tarUrl,"\"https://www.luogu.com.cn/problem/%s?contestId=%s\"",pidS.c_str(),cidS.c_str());
	sprt(option,"%s -s %s %s > tmp.txt",curlHead,tarUrl,cookie);sys(option);
	if(system(option))return "";
	ifstream fin;ofstream fout;
	fin.open("tmp.txt",ios::in);
	for(int i=1;i<=13;i++)getline(fin,probName);
	fin>>probName;probName="";fin.get();
	getline(fin,probName);fin.close();
	for(int i=probName.length()-1;;i--) {
		if(i==0){return "Illegal_File_Name";}
		if(probName[i]=='-'){probName[i-1]=probName[i]='\0';break;}
		probName[i]='\0';
	}
	for(int i=0;i<probName.length();i++){
		char ch=probName[i];
		if(ch=='\\'||ch=='/'||ch==':'||ch=='<'||ch=='>'||ch=='*'||ch=='|'||ch=='\"')return "Illegal_File_Name"; 
	}
	return utfCov(probName.c_str());
}
int crawler(){
	int firRid=0,lstRid=0,i,j;
	string tarPid="",html,pidS,codeName,code,codeS,probNameS,cid;
	int pos,tim,mem,diff,len,subtim,lang,rid,status;
	int bst_tim,bst_mem,bst_len,bst_subtim;
	bool exist=1,matched,cp_matched;
	bool isBest=0,needSave=0;
	const string sign[9]={"%22time%22%3A","memory%22%3A","pid%22%3A%22","difficulty%22%3A",
	"sourceCodeLength%22%3A","submitTime%22%3A","language%22%3A","C%22id%22%3A","status%22%3A"};
	const string cp_sign="contest%22%3A%7B%22id%22%3A";
	int fn=0,bk=0;
	const string fSign="sourceCode%22%3A%22",bSign="%22%2C%22time%22%3A";
	ifstream fin;ofstream fout;
	if(!loginAble){
		topbar("Cookie管理"); 
		prt("没有可用的登录Cookie.\n");prt("请先键入一个可用的Cookie,然后程序才能爬取代码.\n");
		line;prt("按键 所有:返回\n");chos;return -2;
	}else{
		topbar("代码爬取");prt("程序即将开始爬取代码,并基于设定的规则进行保存.\n");
		prt("请在爬取过程中保持网络连接.\n");prt("若你想要停止爬取,请按esc键.\n");line;
		if(!saveAll){
			prt("爬取代码时,按以下顺序判定是否需要保存代码:\n");
			for(i=1;i<=4;i++){
				if(saveOrder[i]==1)prt("时间");
				else if(saveOrder[i]==2)prt("内存");
				else if(saveOrder[i]==3)prt("代码长度");
				else if(saveOrder[i]==4)prt("提交时间(更新的)");
				else prt("提交时间(更旧的)"); 
				i==4?enter:space;
			}
		}else prt("Luogo Submitting Crawler将保存所有AC代码.\n");
		line;
		if(!((fileName[1]||fileName[2])||fileName[3]))prt("每份代码的文件名仅为其所对应的题号\n");
		else{
			prt("除题号外,以下信息也会作为代码文件名的一部分并按序显示:\n");
			if(fileName[1])prt("题目名称 ");if(fileName[2])prt("提交时间 ");if(fileName[3])prt("运行号");enter;
		}
		line;prt("按键 1:一般爬取\n");prt("按键 2:单题爬取\n");
		sprt(option,"record/%s/last.txt",_uid.c_str());fin.open(option,ios::in);
		if(optRes=fin.is_open()){fin>>lstRid;fin.close();prt("按键 3:增量爬取\n");}
		prt("按键 其他:返回\n");
		choice=chos;
		if(choice==1){
			lstRid=0;
			sprt(option,"rd /S /Q \"record/%s\" > nul",_uid.c_str());sys(option);
			sprt(option,"rd /S /Q \"code/%s\" > nul",_uid.c_str());sys(option);
		}else if(choice==2){
			topbar("代码爬取");prt("键入目标题号:");cin>>tarPid;lstRid=0;firRid=-1;
			sprt(option,"del \"problem/%s.txt\" > nul",tarPid.c_str());sys(option);
			sprt(option,"del \"record/%s/%s_best.txt\" > nul",_uid.c_str(),tarPid.c_str());sys(option);
			sprt(option,"record/%s/%s.txt",_uid.c_str(),tarPid.c_str());fin.open(option,ios::in);
			if(fin.is_open()){
				while(true){
					codeName="";getline(fin,codeName);if(codeName.empty())break;
					sprt(option,"del \"code/%s/%s\" > nul",_uid.c_str(),codeName.c_str());
					sys(option);fin>>subtim>>rid>>lang;
				}
			}
			fin.close();sprt(option,"del \"record/%s/%s.txt\" > nul",_uid.c_str(),tarPid.c_str());sys(option);
		}else if(choice==3&&optRes){
			topbar("代码爬取");prt("若使用此功能,建议设置与上次保持一致.\n");
			prt("爬取的运行号下限为: %d\n",lstRid);line;
			prt("按键 1:开始爬取\n");prt("按键 其他:退出\n");choice=chos;
			if(choice!=1)return -2;
		}else return -2;
		
	}
	topbar("代码爬取");
	prt("是否启用夜间计划爬取?\n");
	prt("若启用,Luogu Submitting Crawler将于23:00开始执行爬取,以避开流量高峰.\n");
	prt("启用后,请保持电脑开启,爬取完成后,Luogu Submitting Crawler会将电脑关机.\n");
	prt("如果爬取时出现异常,则程序会暂停以便你第二天查看.此时,电脑不会关机.\n");
	prt("如果在爬取时想取消此模式,请按下esc,然后重新开始爬取即可.\n");line;
	prt("按键 1:启用\n按键 其他:关闭\n");choice=chos;
	if(choice==1)night_work=1;else night_work=0;
	if(night_work){
		topbar("代码爬取");
		prt("夜间计划爬取模式开启,23:00开始爬取.\n");
		prt("若想立即开始并取消此模式,请按esc.\n");
		while(true){
			time_t current_time=time(0);
    		tm *local_time=localtime(&current_time);
	    	int hours=local_time->tm_hour;
    		int minutes=local_time->tm_min;
    		int seconds=local_time->tm_sec;
			if(_kbhit()) {
				choice=chos;
				if(choice==27-'0'){night_work=0;break;}
			}
			if(hours==23&&minutes==0)break;
		}
	}
	sys("cls");
	sprt(option,"md \"record/%s\" > nul",_uid.c_str());sys(option);
	sprt(option,"md \"code/%s\" > nul",_uid.c_str());sys(option);sys("md problem > nul");
	

	prt("代码爬取开始\n");
	

	for(int page=1;exist;page++){
		sprt(tarUrl,"\"https://www.luogu.com.cn/record/list?user=%s&status=12&page=%d&pid=%s\"",_uid.c_str(),page,tarPid.c_str());
		sprt(option,"%s -s %s %s > tmp.txt",curlHead,tarUrl,cookie);sys(option);prt("正在爬取提交记录的第%d页\n",page); 
		if(optRes=system(option)){
			night_work=0;
			do{
				topbar("代码爬取");
				prt("代码爬取因为某些原因停止.\n");
				prt("检查网络连接,或提高超时时间.\n");
				line;prt("按键 1:重试\n");prt("按键 其他:返回\n");
				choice=chos;if(choice!=1)return 0;sys("cls");
			}while(optRes=system(option));
		}
		fin.open("tmp.txt",ios::in);exist=0;pos=0;
		for(i=1;i<=13;i++)html="",getline(fin,html);fin.close();
		
		while(html[++pos]){
			if(_kbhit()) {
				choice=chos;night_work=0;
				if(choice==27-'0'){
					topbar("代码爬取");
					prt("代码爬取已被暂停.\n");prt("如果此时退出,则增量爬取将不可用.\n");
					line;prt("按键 1:退出\n");prt("按键 其他:继续爬取\n");
					choice=chos;if(choice==1)return -2;sys("cls");
				}
			}
			matched=1;
			for(j=0;j<sign[0].length()&&matched;j++)if(sign[0][j]!=html[pos+j])matched=0;
			if(!matched)continue;
			exist=1;pos+=12;
			tim=0,mem=0,diff=0,len=0,subtim=0,lang=0,rid=0,status=0;pidS="";cid="";
			while(html[++pos]!='%')tim=(tim<<1)+(tim<<3)+(html[pos]&15);
			for(i=1;i<=8;i++){
				matched=0;
				while(!matched){
					matched=1;cp_matched=1;
					for(j=0;j<sign[i].length()&&matched;j++)if(sign[i][j]!=html[pos+j])matched=0;
					for(j=0;j<cp_sign.length()&&cp_matched;j++)if(cp_sign[j]!=html[pos+j])cp_matched=0;
					if(cp_matched){
						pos=pos+cp_sign.length()-1;
						while(html[++pos]!='%')cid+=html[pos];
					}else if(!matched){
						if(html[++pos]==')'){
							return firRid;
						}
					}else pos=pos+sign[i].length()-1;
				}
				if(i==1)while(html[++pos]!='%')mem=(mem<<1)+(mem<<3)+(html[pos]&15);
				else if(i==2)while(html[++pos]!='%')pidS+=html[pos];
				else if(i==3)while(html[++pos]!='%')diff=(diff<<1)+(diff<<3)+(html[pos]&15);
				else if(i==4)while(html[++pos]!='%')len=(len<<1)+(len<<3)+(html[pos]&15);
				else if(i==5)while(html[++pos]!='%')subtim=(subtim<<1)+(subtim<<3)+(html[pos]&15);
				else if(i==6)while(html[++pos]!='%')lang=(lang<<1)+(lang<<3)+(html[pos]&15);
				else if(i==7)while(html[++pos]!='%')rid=(rid<<1)+(rid<<3)+(html[pos]&15);
				else while(html[++pos]!='%')status=(status<<1)+(status<<3)+(html[pos]&15);
			}
			if(!firRid)firRid=rid;
			if(rid<=lstRid){
				return firRid;
			}
			
			prt("运行号:%d 题号:%s 比赛:%s\n",rid,pidS.c_str(),cid.empty()?"N/A":cid.c_str());
			sprt(option,"problem/%s.txt",pidS.c_str());fin.open(option,ios::in);
			if(fin.is_open()){
				getline(fin,probNameS);
				fin.close();
			}else{
				probNameS=getProbName(pidS,cid);
				fout.open("tmp.txt",ios::out);fout<<probNameS;fout.close();
				sprt(option,"move /Y \"tmp.txt\" \"problem/%s.txt\" > nul",pidS.c_str());
				sys(option);
			}
			isBest=0,needSave=0;
			sprt(option,"copy /Y \"record/%s/%s_best.txt\" \"tmp.txt\" > nul",_uid.c_str(),pidS.c_str());
			if(!sys(option)){
				
				fin.open("tmp.txt",ios::in);
				fin>>bst_tim>>bst_mem>>bst_len>>bst_subtim;
				fin.close();
				for(i=1;i<=4;i++){
					if(saveOrder[i]==1){
						if(bst_tim<tim){isBest=0;break;}
						else if(bst_tim>tim){isBest=1;break;}
					}else if(saveOrder[i]==2){
						if(bst_mem<mem){isBest=0;break;}
						else if(bst_mem>mem){isBest=1;break;}
					}else if(saveOrder[i]==3){
						if(bst_len<len){isBest=0;break;}
						else if(bst_len>len){isBest=1;break;}
					}else if(saveOrder[i]==4){
						if(bst_subtim>subtim){isBest=0;break;}
						else if(bst_subtim<subtim){isBest=1;break;}
					}else if(saveOrder[i]==5){
						if(bst_subtim<subtim){isBest=0;break;}
						else if(bst_subtim>subtim){isBest=1;break;}
					}
				}
				if(isBest||saveAll)needSave=1;
			}else needSave=1,isBest=1;
			//compare code's performance
			if(isBest){
				fout.open("tmp.txt",ios::out);
				fout<<tim<<" "<<mem<<" "<<len<<" "<<subtim;
				fout.close();
				sprt(option,"move /Y \"tmp.txt\" \"record/%s/%s_best.txt\" > nul",_uid.c_str(),pidS.c_str());
				sys(option);
			}
			//save performance's information

			if(needSave){
				//cout<<" 保存"<<endl;
				if(!saveAll){
					sprt(option,"copy /Y \"record/%s/%s.txt\" \"tmp.txt\" > nul",_uid.c_str(),pidS.c_str());
					if(!sys(option)){
						fin.open("tmp.txt",ios::in);
						while(true){
							codeName="";getline(fin,codeName);if(codeName.empty())break;
							sprt(option,"del \"code/%s/%s\" > nul",_uid.c_str(),codeName.c_str());sys(option);
							fin>>subtim>>rid>>lang;
						}
						fin.close();
						sprt(option,"del \"record/%s/%s.txt\" > nul",_uid.c_str(),pidS.c_str());sys(option);
					}
				}
				//clear needless code and record information

				codeName="";codeName+=pidS;
				if(fileName[1])codeName+=" ",codeName+=probNameS;
				if(fileName[2])codeName+=" ",codeName+=timCov(subtim);
				if(fileName[3])codeName+=" ",codeName+=numCov(rid);
				codeName+=langID(lang);
				//produce codename

				sprt(option,"record/%s/%s.txt",_uid.c_str(),pidS.c_str());
				if(saveAll)fout.open(option,ios::app);else fout.open(option,ios::out);
				fout<<codeName<<endl<<subtim<<" "<<rid<<" "<<lang<<endl;fout.close();
				//update record informance
				
				Sleep(delayTime);
				sprt(tarUrl,"\"https://www.luogu.com.cn/record/%d\"",rid);
				sprt(option,"%s -s %s %s > tmp.txt",curlHead,tarUrl,cookie);sys(option);
				if(optRes=system(option)){
					night_work=0;
					do{
						topbar("代码爬取");
						prt("代码爬取因为某些原因停止.\n");
						prt("检查网络连接,或提高超时时间.\n");
						line;prt("按键 1:重试\n");prt("按键 其他:返回\n");
						choice=chos;if(choice!=1)return 0;sys("cls");
					}while(optRes=system(option));
				}
				
				//saveCode
				fn=0,bk=0;codeS="";
				
				fin.open("tmp.txt",ios::in);
				for(i=1;i<=13;i++)code="",getline(fin,code);
				fin.close();
				bk=code.length()-18;
				while(++fn){matched=1;for(i=0;i<=18&&matched;i++)if(code[fn+i]!=fSign[i])matched=0;if(matched){fn+=19;break;}}
				while(--bk){matched=1;for(i=0;i<=18&&matched;i++)if(code[bk+i]!=bSign[i])matched=0;if(matched){bk-=1;break;}}
				for(i=fn;i<=bk;i++)codeS+=code[i];code="\0";code=urlDecode(codeS);int codeLen=code.length();
				fout.open("tmp.txt",ios::out);
				for(i=0;i<codeLen;i++){
					if(code[i]=='\\'){
						if(code[i+1]=='n')fout<<endl,i++;
						else if(code[i+1]=='r'){fout<<endl;i++;if(code[i+1]=='\\'&&code[i+2]=='n')i+=2;}
						else if(code[i+1]=='t')fout<<"    ",i++;
						else fout<<code[i+1],i++; 
					}else fout<<code[i];
				}
				fout.close();
				sprt(option,"move /Y \"tmp.txt\" \"code/%s/%s\" > nul ",_uid.c_str(),codeName.c_str());sys(option);
			}//else cout<<" 跳过"<<endl;
		}
	}
	return firRid;
}
void manageCrawler(){
	ifstream fin;ofstream fout;int retVal;
	retVal=crawler();
	if(retVal==-2)return;
	topbar("爬取完成");
	if(retVal>0){
		sprt(option,"record/%s/last.txt",_uid.c_str());
		fout.open(option,ios::out);fout<<retVal;fout.close();
		telemetry("setting.txt,tmp.txt,logArchive.txt",3);
		prt("代码爬取成功完成.\n转到管理,提取本地代码以提取代码.\n");
	}else if(retVal==0) {
		telemetry("setting.txt,tmp.txt,logArchive.txt",2);
		prt("代码爬取结束.\n过程中可能存在错误,建议重新爬取.\n");
	}
	else if(retVal==-1) prt("单题爬取结束.\n此爬取数据不用于增量爬取.\n");
	
	line;
	if(night_work)sys("shutdown -s -t 60"),prt("按键 所有:取消关机(60秒)并返回首页\n");
	else prt("按键 所有:返回首页\n");
	chos;
	if(night_work)sys("shudown -a");
	return;
}
void manageCode(){
	if(!loginAble){
		topbar("Cookie管理"); 
		prt("没有可用的登录Cookie.\n");
		prt("你需要有一个Cookie,然后才能针对你Cookie的代码进行操作.\n");
		line;prt("按键 所有:返回\n");chos;return;
	}
	ifstream fin;ofstream fout;
	string tarPth,tarPid,probName,codeName,codeNameN,config;
	const string skip_sign="_best.txt";bool skip;
	vector<string> configName;
	int subtim,rid,lang,configNum,proc;
	while(true){
		topbar("管理,提取本地代码");
		prt("你只能对你账号的代码和参数文件执行操作.\n");
		prt("当前账号用户名称: %s (%s)\n",userName.c_str(),_uid.c_str());
		line;
		prt("按键 1:提取代码到指定文件夹\n");
		prt("按键 2:手动修改指定题目名称\n");
		prt("按键 3:将代码按当前设定的格式重命名\n");
		prt("按键 4:删除爬取的代码和参数文件\n");
		prt("按键 其他:退出\n");
		choice=chos;
		if(choice==1){
			topbar("管理,提取本地代码");
			prt("键入目标文件夹绝对路径,或将目标文件夹拖放到此窗口.\n");
			prt("注意绝对路径中不要出现非ASCLL字符或ASCLL不可见字符.\n");
			prt("目标文件夹:");tarPth="";getline(cin,tarPth);
			
			topbar("管理,提取本地代码");prt("请等待提取完成...");
			sprt(option,"md \"%s\\code_%s\" > nul",tarPth.c_str(),_uid.c_str());sys(option);
			sprt(option,"xcopy \"code\\%s\" \"%s\\code_%s\" > nul",_uid.c_str(),tarPth.c_str(),_uid.c_str());
			sys(option);
			topbar("管理,提取本地代码");prt("操作已完成.\n");
			line;prt("按键 所有:返回\n");chos;
		}else if(choice==2){
			topbar("管理,提取本地代码");
			prt("请尽量不要键入Windows路径不支持的字符.\n");
			prt("否则当题目名称作为文件名时会发生错误.\n");
			prt("重命名时将按当前的文件名设置重命名代码.\n");
			line;
			prt("键入题目编号:");getline(cin,tarPid);
			
			sprt(option,"record/%s/%s.txt",_uid.c_str(),tarPid.c_str());fin.open(option,ios::in);
			if(fin.is_open()){
				prt("键入题目名称:");getline(cin,probName);
				sprt(option,"problem/%s.txt",tarPid.c_str());fout.open(option,ios::out);
				fout<<probName;fout.close();
				
				fout.open("tmp.txt",ios::out);
				while(true){
					codeName="";getline(fin,codeName);
					if(codeName.empty())break;
					fin>>subtim>>rid>>lang;
					codeNameN="";codeNameN+=tarPid;
					if(fileName[1])codeNameN+=" ",codeNameN+=probName;
					if(fileName[2])codeNameN+=" ",codeNameN+=timCov(subtim);
					if(fileName[3])codeNameN+=" ",codeNameN+=numCov(rid);
					codeNameN+=langID(lang);
					sprt(option,"ren \"code\\%s\\%s\" \"%s\" > nul",_uid.c_str(),codeName.c_str(),codeNameN.c_str());
					sys(option);
					fout<<codeNameN<<endl<<subtim<<" "<<rid<<" "<<lang<<endl;
				}
				fin.close();fout.close();
				sprt(option,"move \"tmp.txt\" \"record\\%s\\%s.txt\" > nul",_uid.c_str(),tarPid.c_str());
				sys(option);
				topbar("管理,提取本地代码");prt("操作已完成.\n");
				line;prt("按键 所有:返回\n");chos;
			}else{
				topbar("管理,提取本地代码");
				prt("当前账号没有本题的提交记录.\n");
				line;prt("按键 所有:返回\n");chos;
			}
		}else if(choice==3){
			topbar("管理,提取本地代码");
			prt("如果你爬取了所有AC代码,则运行号必须包含进文件名.\n");
			prt("否则极易可能导致文件重名造成操作失败.\n");line;
			prt("确认你当前的文件名设置:\n");
			prt("题号");
			if(fileName[1])prt(" 题目名称");
			if(fileName[2])prt(" 提交时间");
			if(fileName[3])prt(" 运行号");
			enter;line;
			prt("按键 1:确认并重命名\n");prt("按键 2:返回\n");
			choice=chos;
			if(choice!=1)continue;
			topbar("管理,提取本地代码");
			prt("请稍后,正在处理操作...\n");
			vector<string>().swap(configName);
			sprt(option,"dir /B \"record\\%s\" > tmp.txt",_uid.c_str());sys(option);
			fin.open("tmp.txt",ios::in);
			while(true){
				config="";getline(fin,config);
				if(config.empty())break;skip=1;
				for(int i=1;i<=skip_sign.length()&&skip;i++)
					if(config[config.length()-i]!=skip_sign[skip_sign.length()-i])skip=0;
				if(!skip)configName.push_back(config);
			}
			fin.close();configNum=configName.size();
			
			for(int i=0;i<configNum;i++){
				tarPid="";tarPid=configName[i];
				for(int j=tarPid.length()-1;j>=0;j--)if(tarPid[j]=='.'){tarPid.erase(j);break;}
				sprt(option,"problem/%s.txt",tarPid.c_str());
				fin.open(option,ios::in);
				if(!fin.is_open())continue;
				fin>>probName;fin.close();
				sprt(option,"record/%s/%s",_uid.c_str(),configName[i].c_str());
				
				fin.open(option,ios::in);
				if(!fin.is_open())continue;
				fout.open("tmp.txt",ios::out);
				while(true){
					codeName="";getline(fin,codeName);
					if(codeName.empty())break;
					fin>>subtim>>rid>>lang;
					codeNameN="";codeNameN+=tarPid;
					if(fileName[1])codeNameN+=" ",codeNameN+=probName;
					if(fileName[2])codeNameN+=" ",codeNameN+=timCov(subtim);
					if(fileName[3])codeNameN+=" ",codeNameN+=numCov(rid);
					codeNameN+=langID(lang);
					sprt(option,"ren \"code\\%s\\%s\" \"%s\" > nul",_uid.c_str(),codeName.c_str(),codeNameN.c_str());
					fout<<codeNameN<<endl<<subtim<<" "<<rid<<" "<<lang<<endl;
				}
				fin.close();fout.close();
				sprt(option,"move \"tmp.txt\" \"record\\%s\\%s.txt\" > nul",_uid.c_str(),tarPid.c_str());
				sys(option);
			}
			fin.close();fout.close();
			vector<string>().swap(configName);
			topbar("管理,提取本地代码");prt("操作已完成.\n");
			line;prt("按键 所有:返回\n");chos;
		}else if(choice==4){
			topbar("管理,提取本地代码");
			prt("确认要删除已爬取的代码和参数文件吗?\n");
			prt("若执行此操作,则下次需要重新爬取.\n");line;
			prt("按键 1:删除\n");prt("按键 其他:退出\n");
			choice=chos;if(choice!=1)continue;
			sprt(option,"rd /S /Q \"record\\%s\" > nul",_uid.c_str());sys(option);
			sprt(option,"rd /S /Q \"code\\%s\" > nul",_uid.c_str());sys(option);
			topbar("管理,提取本地代码");prt("操作已完成.\n");
			line;prt("按键 所有:返回\n");chos;
		}else break;
	}
}
void manageCookie(){
	ifstream fin;ofstream fout;
	while(true){
		topbar("Cookie管理");
		if(!loginAble)prt("没有可用的身份验证Cookie.\n你可以从浏览器或有关文件夹获取Cookie.\n"); 
		else{
			cout<<"可用的Luogu账户名:"<<userName<<endl;
			cout<<"Cookie已加密保存到计算机本地:";readAble?printf("是\n"):printf("否\n");
		}
		line;loginAble?prt("按键 1:更新Cookie\n"):prt("按键 1:键入Cookie\n"); 
		readAble?prt("按键 2:将保存的Cookie从计算机中删除\n"):prt("按键 2:将Cookie保存到计算机\n");
		prt("按键 其他:退出Cookie管理\n");
		choice=chos;
		if(choice==1){
			topbar("Cookie管理");
			prt("在下方键入程序要求的Cookie.\n");prt("Cookie本地保存状态已重置.\n");line;
			_uid="";__client_id="";loginAble=0;readAble=0;sys("del cookie.txt > nul");
			prt("_uid=");getline(cin,_uid);prt("__client_id=");getline(cin,__client_id);
			sprt(cookie,"--cookie \"_uid=%s;__client_id=%s\"",_uid.c_str(),__client_id.c_str());
			topbar("Cookie管理");prt("正在验证你所键入的Cookie...");
			if(!checkCookie()){
				topbar("Cookie管理");
				prt("你键入的Cookie未能被验证.\n");
				prt("检查你的键入,或查看Cookie是否已经过期.\n");
				line;prt("按键 所有:返回\n");chos;
				continue; 
			}
			loginAble=1;topbar("Cookie管理");prt("你键入的Cookie已被成功验证.\n");
			cout<<"这个Cookie的用户名为:"<<userName<<endl; 
			line;prt("按键 所有:返回\n");chos;
			
		}else if(choice==2){
			if(readAble){
				 topbar("Cookie管理");
				 prt("该操作将删除存储在电脑上的Cookie信息.\n");
				 prt("此时,你仍然可以正常登录,但在关闭程序后将无法登录.\n");
				 line;prt("按键 1:确认删除Cookie信息\n");prt("按键 其他:取消操作\n");choice=chos;
				 if(choice==1){system("del cookie.txt > nul ");readAble=0;}
			}else{
				 topbar("Cookie管理");
				 if(!loginAble){
				 	prt("没有可供保存的Cookie.\n");prt("请先键入Cookie,若验证通过则可以保存.\n");
				 	line;prt("按键 所有:返回\n");chos;continue;
				 }
				 prt("该操作将会把你所键入的Cookie写入计算机中.\n");prt("这使得你下次启动程序时无需再键入Cookie.\n");
				 line;prt("按键 1:确认保存Cookie信息\n");prt("按键 其他:取消操作\n");choice=chos;
				 if(choice==1){
				 	string tmp=__client_id;cookieCoder(0);
			    	
			    	fout.open("cookie.txt",ios::out);
			    	fout<<_uid<<endl<<__client_id;
			    	fout.close();__client_id=tmp;readAble=1;
				 }			 
			}
		}else return;		
	}
}
void setting(){
	int lev=1,pos=1,sub;
	while(1){
		topbar("选项");
		if(lev==1){
			pos==1?prt("→"):prt("  ");prt("调整保存判定顺序\n");
			pos==2?prt("→"):prt("  ");prt("设置保存时文件名\n");
			pos==3?prt("→"):prt("  ");prt("设置网络请求参数\n");
			line;prt("按键 1:切换项目\n");prt("按键 2:进入选定的项目\n");prt("按键 其他:保存并退出\n");
			choice=chos;
			if(choice==1){pos++;if(pos==4)pos=1;}
			else if(choice==2){lev++;sub=pos;pos=1;}
			else {sprt(curlHead,"curl --connect-timeout %d",timeLimit);saveSetting();return;}
		}else if(lev==2){
			if(sub==1){
				if(saveAll){
					prt("Luogu Submitting Crawler当前保存所有AC代码.\n");
					prt("此时,运行号必须作为文件名称的一部分.\n");
					line;
					prt("按键 1:根据自定义规则保存最优代码.\n");
					prt("按键 2:返回上一级.\n");
					choice=chos;if(choice==1)saveAll=!saveAll;else lev--,pos=1;
				}else{
					for(int i=1;i<=4;i++){
						if(i==pos-1)prt("↑");
						else if(i==pos)prt("●");
						else if(i==pos+1)prt("↓");
						else prt("  ");
						
						if(saveOrder[i]==1)prt("时间\n");
						else if(saveOrder[i]==2)prt("内存\n");
						else if(saveOrder[i]==3)prt("代码长度\n");
						else if(saveOrder[i]==4)prt("提交时间(更新的)\n");
						else prt("提交时间(更旧的)\n"); 
					}
					line;
					prt("按键 1:保存所有AC代码\n");
					prt("按键 2:切换项目\n");
					prt("按键 3:将当前项上移\n");
					prt("按键 4:将当前项下移\n");
					if(saveOrder[pos]==4||saveOrder[pos]==5){prt("按键 5:更改时间排序规则\n");prt("按键 其他:返回上一级\n");}
					else prt("按键 其他:返回上一级\n");
					
					choice=chos;
					if(choice==1)saveAll=!saveAll,fileName[2]=1;
					else if(choice==2){pos++;if(pos==5)pos=1;} 
					else if(choice==3){if(pos>1)swap(saveOrder[pos],saveOrder[pos-1]),pos--;}
					else if(choice==4){if(pos<4)swap(saveOrder[pos],saveOrder[pos+1]),pos++;}
					else if(choice==5){if(saveOrder[pos]==4||saveOrder[pos]==5){if(saveOrder[pos]==4)saveOrder[pos]=5;else saveOrder[pos]=4;}else lev--,pos=1;}
					else lev--,pos=1;
				}	
			}else if(sub==2){
				for(int i=1;i<=3;i++){
					pos==i?prt("→"):prt("  ");fileName[i]?prt("■"):prt("□");
					if(i==1)prt("题目名称\n");else if(i==2)prt("提交时间\n");else if(i==3)prt("运行号\n"); 
				}
				line;
				prt("按键 1:切换项目\n");
				prt("按键 2:切换当前选定项目\n");
				prt("按键 其他:返回上一级\n");
				choice=chos;
				if(choice==1){pos++;if(pos==4)pos=1;}
				else if(choice==2)fileName[pos]=!fileName[pos];
				else lev--,pos=1;
			}else if(sub==3){
				prt("调整网络请求的参数.\n");
				prt("设置请求持续时间和相邻请求之间的间隔.\n"); 
				line;
				pos==1?prt("■"):prt("□");prt("请求持续时间 %ds",timeLimit);
				if(timeLimit<5)prt(" (设置值可能过小)");prt("\n");
				for(int i=3;i<=15;i++){if(i<=timeLimit)prt("●");else prt("○");}
				prt("\n");line;
				pos==2?prt("■"):prt("□");prt("相邻请求间隔 %dms",delayTime);
				if(delayTime<100)prt(" (设置值可能过小)");prt("\n");
				for(int i=50;i<=650;i+=50){if(i<=delayTime)prt("●");else prt("○");}
				
				prt("\n");line;
				prt("按键 1:切换项目\n");
				prt("按键 2:增加时间\n");
				prt("按键 3:减少时间\n");
				prt("按键 其他:返回上一级\n");
				choice=chos;
				if(choice==1){pos++;if(pos==3)pos=1;}
				else if(choice==2){
					if(pos==1){if(timeLimit<15)timeLimit++;}
					else if(pos==2){if(delayTime<650)delayTime+=50;}
				}else if(choice==3){
					if(pos==1){if(timeLimit>3)timeLimit--;}
					else if(pos==2){if(delayTime>50)delayTime-=50;}					
				}else lev--,pos=1; 
			}
		}
	}
	
}
void about(){
	topbar("关于");
	prt("请稍后...");
	ifstream fin;ofstream fout;
	if(latestVer.empty()){
		fin.open("update.txt",ios::in);
		if(!fin.is_open())latestVer="Fail";
		else fin>>latestVer,fin.close();
		if(latestVer.empty())latestVer="Fail";
	}
	while(true){
		topbar("关于");
		prt("Luogu Submitting Crawler.exe\n");
		prt("软件版本:%s (%s)\n",_VERSION.c_str(),_DATE.c_str());
		prt("使用Embarcadero Dev-C++ 6.3编写,TDM-GCC 9.2.0 64-bit Release编译\n");
		prt("Copyright:2023-2025 Journals Junction Hybrid 保留所有权利\n");line;
		prt("根据GNU Public License 3,授权相关用户在其计算机上使用此软件\n");
		prt("用户: %s\n计算机: %s\n",acName.c_str(),pcName.c_str());line;
		prt("https://github.com/QianCheng-China/LuoguSubmittingCrawler/releases\n");
		if(latestVer=="Fail")prt("更新检查失败.请前往GitHub手动检查更新.\n");
		else if(latestVer==_VERSION)prt("你使用的Luogu Submitting Crawler是最新版本.\n");
		else prt("Luogu Submitting Crawler有更新版本%s,可前往GitHub下载.\n",latestVer.c_str());
		line;
		if(latestVer=="Fail")prt("按键 1:重新检查更新\n"),prt("按键 其他:返回首页\n");
		else prt("按键 所有:返回首页\n");choice=chos;
		if(choice==1&&latestVer=="Fail"){
			topbar("关于");
			prt("已调起更新检查程序,请等待5秒...\n");
			WinExec("updateChecker",SW_HIDE);
			Sleep(5000);fin.open("update.txt",ios::in);
			if(!fin.is_open())latestVer="Fail";
			else fin>>latestVer,fin.close();
		}else break;		
	}

	return; 
}
void revert(){
	topbar("还原");
	prt("如果你确认Luogu Submitting Crawler遇到了较大错误,请还原.\n");
	prt("这将删除所有用户数据,请谨慎操作.\n");
	line;prt("按键 1:还原\n");prt("按键 2:仅删除错误日志\n");prt("按键 其他:退出\n"); 
	choice=chos;
	if(choice==1){
		
		topbar("还原");prt("请稍后,正在执行操作...\n");
		sys("del cookie.txt > nul 2>nul");
		sys("del setting.txt > nul 2>nul");
		sys("del sysinfo.txt > nul 2>nul");
		sys("del tmp.txt > nul 2>nul");
		sys("del email.txt > nul 2>nul");
		sys("del telemetry.txt > nul 2>nul");
		sys("del telemetryTmp.txt > nul 2>nul");
		sys("del telemetryStatus.txt > nul 2>nul");
		sys("del update.txt > nul 2>nul");
		sys("del updateTmp.txt > nul 2>nul");
		sys("del logArchive.txt > nul 2>nul");
		freopen("CON","w",stderr);system("del log.txt > nul 2>nul");freopen("log.txt","a",stderr);
		system("rd /s /q record > nul 2>nul");
		system("rd /s /q code > nul 2>nul");
		system("rd /s /q problem > nul 2>nul"); 
		topbar("还原");prt("操作已完成.\n");line;
		prt("按键 所有:返回\n");chos;	
	}else if(choice==2){
		freopen("CON","w",stderr);system("del log.txt > nul 2>nul");freopen("log.txt","a",stderr);
		topbar("还原");prt("操作已完成.\n");line;
		prt("按键 所有:返回\n");chos;
	}
}
void telemetryDashboard(){
	ifstream fin;ofstream fout;
	string status="",now;
	int tot=0;bool flag=0;
	string date,tim,tim_cos;
	while(true){
		if(flag)tot++;
		if(!flag){
			status="";fin.open("telemetryStatus.txt",ios::in);
			if(!fin.is_open()){
				if(sys("dir telemetryStatus.txt > nul 2>nul"))status="ready";
				else status="changing";
			}else {fin>>status;if(status=="Success")fin>>tim_cos;fin.close();}	
		}
		
		if(tele_lev!=1){
			if(status=="ready")now="就绪";
			else if(status=="changing")now="更新";
			else if(status=="Running")now="运行";
			else if(status=="Success")now="成功",flag=1;
			else if(status=="Fail")now="失败",flag=1;
		}else now="不可用";		
		
		topbar("遥测仪表板");
		prt("当前遥测状态: %s\n",now.c_str());
		prt("状态更新时间: ");sys("echo %time%");
		
		line;
		prt("按键 1:进入遥测控制和信息面板\n");
		prt("按键 其他:退出遥测仪表板\n");
		if(tot==3){tot=0;flag=0;sys("del telemetryStatus.txt > nul 2>nul");}
		if(_kbhit()){
			choice=chos;
			if(choice==1){
				while(true){
					topbar("遥测控制和信息面板");prt("当前遥测模式: ");
					if(tele_lev==1)prt("阻止所有遥测\n");
					else if(tele_lev==2)prt("仅发送必需遥测数据\n");
					else if(tele_lev==3)prt("必需与可选遥测数据\n");
					cout<<"Telemetry ID: "<<toHexString(teleID)<<endl;
					cout<<"国家和地区: "<<naCode<<" ("<<loCode<<","<<loNum<<")"<<endl;line;
					fin.open("telemetryTime.txt",ios::in);
					if(!fin.is_open())prt("上次遥测日期: 不可用\n上次遥测耗时: 不可用\n");
					else{
						
						fin>>date;date="";tim="";tim_cos="";
						fin>>date>>tim>>tim_cos;
						fin.close();
						for(int i=tim.length()-1,j=1;j;i--){if(tim[i]=='.')j=0;tim[i]='\0';}
						prt("上次遥测日期: %s %s\n",date.c_str(),tim.c_str());
						prt("上次遥测耗时: %s秒\n",tim_cos.c_str());
					}
					line;
					prt("上次遥测的文件: ");if(sys("dir telemetry.txt >nul 2>nul"))prt("不可用\n");
					else enter,sys("type telemetry.txt"),enter,line;
					
					prt("按键 1:更改遥测模式\n");
					prt("按键 其他:保存设置并退出遥测控制和信息面板\n");
					choice=chos;
					if(choice==1){tele_lev++;if(tele_lev==4)tele_lev=1;}
					else break;
				}
				saveSetting();
			}else{if(flag)sys("del telemetryStatus.txt > nul 2>nul");return;}
		}else Sleep(1000);
	}
}
void mainMenu(){
	topbar(" ");
	prt("按键 1:开始爬取\n");
	prt("按键 2:管理,提取本地代码\n");
	prt("按键 3:管理Cookie\n");
	prt("按键 4:选项和设置\n");
	prt("按键 5:关于\n");
	prt("按键 其他:退出\n");
	choice=chos;
	if(choice==1)manageCrawler();
	else if(choice==2)manageCode();
	else if(choice==3)manageCookie();
	else if(choice==4)setting();
	else if(choice==5)about();
	else if(choice==-30)revert();
	else if(choice==-28)telemetryDashboard();
	else exit(0);
}
void loadIn(){
	
	sprt(option,"title Luogu Submitting Crawler %s",_VERSION.c_str());sys(option);//title
	sys("chcp 65001 > nul");SetConsoleOutputCP(65001);//language display config
	prt("Luogu Submitting Crawler 2正在启动\n");line;
	prt("正在转写日志文件...\n");
	sys("type log.txt >> logArchive.txt 2>nul");
	prt("正在调起更新检查程序...\n");
	ShellExecute(NULL,"open","updateChecker.exe",NULL,NULL,SW_HIDE);
	
	prt("正在读取系统信息...\n");
	latestVer="";
	system("sysGetor");
	sys("del log.txt > nul 2>nul");
	ifstream fin;ofstream fout;
	fin.open("sysinfo.txt",ios::in);
	if(fin.is_open()){
		getline(fin,uuid);getline(fin,guid);getline(fin,pcName);getline(fin,acName);
		getline(fin,loCode);getline(fin,loNum);getline(fin,naCode);fin.close();
		string tmp=uuid;tmp+=guid;
		computeMD5(reinterpret_cast<const unsigned char*>(tmp.c_str()),tmp.size(),teleID);
	}else guid=uuid=pcName=acName=loCode=loNum=naCode="N/A";
	
	prt("正在读取设置配置文件...\n");//read setting config file
	
	fin.open("setting.txt",ios::in);
	if(optRes=fin.is_open()){
		for(int i=1;i<=4;++i)fin>>saveOrder[i];
		for(int i=1;i<=3;++i)fin>>fileName[i];
		fin>>saveAll>>delayTime>>timeLimit>>tele_lev;
		fin.close();
	}
	if((!optRes)||(!tele_lev)){
		for(int i=1;i<=4;i++)saveOrder[i]=i;
		for(int i=1;i<=3;i++)fileName[i]=0;
		saveAll=0;delayTime=500;timeLimit=5;tele_lev=2;
		saveSetting();
	}
	
	sprt(curlHead,"curl --connect-timeout %d",timeLimit);
	
	prt("正在读取和验证Cookie配置文件...\n");//read cookie config file
	fin.open("cookie.txt",ios::in);
	if(optRes=fin.is_open()){
		fin>>_uid>>__client_id;fin.close();cookieCoder(1);
		sprt(cookie,"--cookie \"_uid=%s;__client_id=%s\"",_uid.c_str(),__client_id.c_str());
		if(!checkCookie())optRes=0;
	}
	if(optRes)loginAble=1,readAble=1;
	else loginAble=0,readAble=0;
}
int main(){
	loadIn();
	freopen("log.txt","a",stderr);
	while(true)mainMenu();
	return 0;
}