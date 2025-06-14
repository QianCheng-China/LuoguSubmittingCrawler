#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<io.h>
#include<string>
#include<fstream> 

#define MAX_COOKIE 100
#define MAX_SYSINFO 100

#define STDIN 0
#define STDOUT 1
#define STDERR 2 
using namespace std;
const int oldstdout=dup(STDOUT),oldstdin=dup(STDIN),oldstderr=dup(STDERR);
void cloOut(){fclose(stdout);freopen("CON","w",stdout);dup2(oldstdout,STDOUT);}
void cloIn(){fclose(stdin);freopen("CON","r",stdin);dup2(oldstdin,STDIN);}

int choose;
const string version="1.1.1",date="June 15th 2025";

char cmptUser[MAX_SYSINFO],cmptName[MAX_SYSINFO],GID[MAX_SYSINFO],UID[MAX_SYSINFO];
string localID,localName;


//account part
char _uid[MAX_COOKIE],__client_id[MAX_COOKIE];
string userName;
bool loginAble,localSave;

char tarUrl[1000],option[2000];
int timLim,optionRet;
int delayTim;
int firRid,lstRid;
int order[5];
//1=tim,2=mem,3=len,4=subtim(newer),5=subtim(older) 

bool fileName[5];
//1st=name,2nd=subTim,3rd=rid


void line(){printf("--------------\n");}
void topbar(string s){
	system("cls");
	printf("Luogu Submitting Crawler\n");
	printf("首页");
	if(s!=" ")printf("-"),cout<<s;
	printf("\n");line();
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
	delete[] strSrc;
	delete[] szRes;
	return result;
}
string getProbName(string pidS) {
	string probName;
	sprintf(tarUrl,"\"https://www.luogu.com.cn/problem/%s\"",pidS.c_str());
	sprintf(option,"curl --connect-timeout %d -s %s --cookie \"_uid=%s;__client_id=%s\" > tmp.txt",timLim,tarUrl,_uid,__client_id);
	if(system(option))return "";
	freopen("tmp.txt","r",stdin);
	for(int i=1; i<=13; i++)getline(cin,probName);
	cin>>probName;probName="\0";getchar();
	getline(cin,probName);cloIn();
	for(int i=probName.length()-1;;i--) {
		if(probName[i]=='-'){probName[i-1]=probName[i]='\0';break;}
		probName[i]='\0';
	}
	return utfCov(probName.c_str());
}
void getCodeFinal(){
	cloIn(); 
	freopen("tmp.txt","w",stdout);
	printf("%d",firRid);
	cloOut();
	sprintf(option,"move /Y tmp.txt \"history\\%s\\latestRid.txt\" > nul ",_uid);system(option);
	topbar("代码爬取");
	printf("代码爬取已完成.\n");
	printf("下一次,增量更新将可以使用.\n");
	line();
	printf("按键 所有:返回\n");
	_getch();
	return;
}
bool getCode(){
	 firRid=0;lstRid=0;
	if(!loginAble){
		topbar("Cookie管理"); 
		printf("没有可用的登录Cookie.\n");
		printf("请先键入一个可用的Cookie,然后程序才能爬取代码.\n");
		line();
		printf("按键 所有:返回\n");
		_getch();
		return 1;
	}else{
		topbar("代码爬取");
		printf("程序即将开始爬取代码,并基于设定的规则进行保存.\n");
		printf("请在爬取过程中保持网络连接.\n");
		printf("若你想要停止爬取,请按esc键.\n");
		line();
		printf("爬取代码时,按以下顺序判定是否需要保存代码:\n");
		for(int i=0;i<4;i++){
			if(order[i]==1)printf("时间");
			else if(order[i]==2)printf("内存");
			else if(order[i]==3)printf("代码长度");
			else if(order[i]==4)printf("提交时间(更新的)");
			else printf("提交时间(更旧的)"); 
			i==3?printf("\n"):printf(" ");
		}
		line();
		if(!(fileName[0]||fileName[1]))printf("每份代码的文件名仅为其所对应的题号\n");
		else{
			printf("除题号外,以下信息也会作为代码文件名的一部分并按序显示:\n");
			if(fileName[0])printf("题目名称 ");
			if(fileName[1])printf("提交时间 "); 
			if(fileName[2])printf("运行号");
			printf("\n"); 
		}
		line();
		printf("按键 1:开始一般爬取\n");
		printf("按键 2:开始增量爬取\n");
		printf("按键 其他:返回\n");
		choose=_getch()-'0';
		if(choose!=1&&choose!=2)return 1;
		if(choose==2){
			topbar("代码爬取");
			printf("请稍后...");
			sprintf(option,"dir \"history\\%s\\latestRid.txt\" > nul ",_uid);
			
			if(system(option)){
				topbar("代码爬取");
				printf("增量爬取当前不可用.\n");
				printf("你可以切换到一般爬取,也可以退出.\n");
				line();
				printf("按键 1:切换到一般爬取.\n");
				printf("按键 其他:退出\n");
				choose=_getch()-'0';
				if(choose!=1)return 1;
			}else{
				sprintf(option,"copy /Y \"history\\%s\\latestRid.txt\" tmp.txt > nul ",_uid);optionRet=system(option);
				freopen("tmp.txt","r",stdin);
				scanf("%d",&lstRid);
				cloIn();system("del tmp.txt");
				topbar("代码爬取");
				printf("增量爬取目前可用.\n");
				printf("运行号下限为:%d\n",lstRid);
				line();
				printf("按键 所有:开始爬取\n");
				_getch();
			}
			 
		}
	}
	system("cls");
	
	const string sign[9]={"%22time%22%3A","memory%22%3A","pid%22%3A%22","difficulty%22%3A","sourceCodeLength%22%3A","submitTime%22%3A","language%22%3A","C%22id%22%3A","status%22%3A"};
	string html,pidS,codeNameS,code,codeS,probNameS;
	bool exist=1,matched,save;
	int pos,tim,mem,diff,len,subtim,lang,rid,status;
	
	printf("代码爬取开始\n");
	if(!lstRid)sprintf(option,"rd /S /Q \"code\\%s\" > nul ",_uid);system(option);
	sprintf(option,"md \"code\\%s\" > nul ",_uid);system(option);
	
	if(!lstRid)sprintf(option,"rd /S /Q \"history\\%s\" > nul ",_uid);system(option); 
	sprintf(option,"md \"history\\%s\" > nul ",_uid);system(option);
	
	for(int page=1;exist;page++){
		printf("正在爬取提交记录的第%d页\n",page); 
		sprintf(tarUrl,"\"https://www.luogu.com.cn/record/list?user=%s&page=%d\"",_uid,page);
		sprintf(option,"curl --connect-timeout %d -s %s --cookie \"_uid=%s;__client_id=%s\" > tmp.txt",timLim,tarUrl,_uid,__client_id);
		optionRet=system(option);
		if(optionRet){
			topbar("代码爬取");
			printf("代码爬取因为某些原因停止.\n");
			printf("检查网络连接,或提高超时时间.\n");
			line();
			printf("按键 所有:返回\n");
			_getch();
			return 1;
		}
		freopen("tmp.txt","r",stdin);exist=0;pos=0;
		for(int i=1;i<=13;i++)html="\0",getline(cin,html);
		cloIn();system("del tmp.txt > nul ");
		while(html[++pos]){
			if(_kbhit()) {
				choose=_getch();
				if(choose==27){
					topbar("代码爬取");
					printf("代码爬取已被暂停.\n");
					printf("如果此时退出,则增量爬取将不可用.\n");
					line();
					printf("按键 1:退出\n");
					printf("按键 其他:继续爬取\n");
					choose=_getch()-'0';
					if(choose==1)return 1;
					system("cls");
				}
			}
			
			matched=1;
			for(int j=0;j<sign[0].length()&&matched;j++)if(sign[0][j]!=html[pos+j])matched=0;
			if(!matched)continue;
			exist=1;pos+=12;
			tim=0,mem=0,diff=0,len=0,subtim=0,lang=0,rid=0,status=0;pidS="\0";
			while(html[++pos]!='%')tim=(tim<<1)+(tim<<3)+(html[pos]&15);
			for(int i=1;i<=8;i++){
				matched=0;
				while(!matched){
					matched=1;
					for(int j=0;j<sign[i].length()&&matched;j++)if(sign[i][j]!=html[pos+j])matched=0;
					if(!matched){
						if(html[++pos]==')')return 0;
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
			if(rid<=lstRid){return 0;}
			printf("运行号:%d 题号:%s 时间:%d 内存:%d 难度:%d 代码长度:%d 提交时间:%d 语言:%d 状态:%d\n",rid,pidS.c_str(),tim,mem,diff,len,subtim,lang,status);
			save=0;
			
			sprintf(option,"copy /Y \"history\\%s\\%s.txt\" tmp.txt > nul ",_uid,pidS.c_str());optionRet=system(option);
			
			if(!optionRet){
				freopen("tmp.txt","r",stdin);
				probNameS="\0";getline(cin,probNameS);
				codeNameS="\0";getline(cin,codeNameS);
				for(int i=0,val;i<=3;i++){
					scanf("%d",&val);
					if(order[i]==1)save|=(val>=tim);
					else if(order[i]==2)save|=(val>=mem);
					else if(order[i]==3)save|=(val>=len);
					else if(order[i]==4)save|=(val<subtim);
					else save|=(val>subtim);
				}
				cloIn();
			}else probNameS=getProbName(pidS);
			
			if((!save)||(status!=12)){
				if(optionRet){
					freopen("tmp.txt","w",stdout);
					printf("%s\n-\n0 0 0 0 0",probNameS.c_str());
					cloOut();
					sprintf(option,"move /Y \"tmp.txt\" \"history\\%s\\%s.txt\" > nul ",_uid,pidS.c_str());system(option);
				}
				printf("该提交记录的代码保存已跳过\n");
			}else{
				if((!optionRet)&&codeNameS[0]=='-'){sprintf(option,"del \"code\\%s\\%s\" > nul ",_uid,codeNameS.c_str());system(option);}
				
				codeNameS="\0";codeNameS+=pidS;
				if(fileName[0])codeNameS+=" ",codeNameS+=probNameS;
				if(fileName[1])codeNameS+=" ",codeNameS+=timCov(subtim);
				if(fileName[2])codeNameS+=" ",codeNameS+=numCov(rid);
				codeNameS+=langID(lang);
				
				
				printf("该提交记录代码将被保存为: %s\n",codeNameS.c_str());
				
				Sleep(delayTim);
				sprintf(tarUrl,"\"https://www.luogu.com.cn/record/%d\"",rid);
				sprintf(option,"curl --connect-timeout %d -s %s --cookie \"_uid=%s;__client_id=%s\" > tmp.txt",timLim,tarUrl,_uid,__client_id);
				optionRet=system(option);
				if(optionRet){
					topbar("代码爬取");
					printf("代码爬取因为某些原因停止.\n");
					printf("检查网络连接,或提高超时时间.\n");
					line();
					printf("按键 所有:返回\n");
					_getch();
					return 1;
				}
				
				//saveCode
				int fn=0,bk=0;codeS="\0";
				const string fSign="sourceCode%22%3A%22",bSign="%22%2C%22time%22%3A";
				freopen("tmp.txt","r",stdin);
				for(int i=1;i<=13;i++)code="\0",getline(cin,code);
				cloIn();system("del tmp.txt > nul ");
				bk=code.length()-18;
				while(++fn){
					matched=1;
					for(int i=0;i<=18&&matched;i++)if(code[fn+i]!=fSign[i])matched=0;
					if(matched){fn+=19;break;}
				}
				while(--bk){
					matched=1;
					for(int i=0;i<=18&&matched;i++)if(code[bk+i]!=bSign[i])matched=0;
					if(matched){bk-=1;break;}
				}
				for(int i=fn;i<=bk;i++)codeS+=code[i];
				code="\0";code=urlDecode(codeS);
				int codeLen=code.length();
				freopen("tmp.txt","w",stdout);
				for(int i=0;i<codeLen;i++){
					if(code[i]=='\\'){
						if(code[i+1]=='n')putchar('\n'),i++;
						else if(code[i+1]=='r'){
							putchar('\n');i++;
							if(code[i+1]=='\\'&&code[i+2]=='n')i+=2;
						}
						else if(code[i+1]=='t')printf("    "),i++;
						else putchar(code[i+1]),i++; 
					}else putchar(code[i]);
				}
				cloOut();
				sprintf(option,"move /Y \"tmp.txt\" \"code\\%s\\%s\" > nul ",_uid,codeNameS.c_str());system(option);
		
				//saveResult
				freopen("tmp.txt","w",stdout);
				printf("%s\n%s\n%d %d %d %d %d\n",probNameS.c_str(),codeNameS.c_str(),tim,mem,len,subtim,rid);
				printf("%d %d %d",fileName[0],fileName[1],fileName[2]);
				cloOut();
				sprintf(option,"move /Y \"tmp.txt\" \"history\\%s\\%s.txt\" > nul ",_uid,pidS.c_str());system(option);
		
			}
		}
	}
	
}
bool checkCookie(){
	sprintf(tarUrl,"\"https://www.luogu.com.cn/\"");
	sprintf(option,"curl --connect-timeout %d -s %s --cookie \"_uid=%s;__client_id=%s\" > tmp.txt",timLim,tarUrl,_uid,__client_id);system(option);
			
	int fn=0,bk=0,len;
	string html;
	bool matched=0;
			
	freopen("tmp.txt","r",stdin);
	for(int i=1;i<=23;i++)html="\0",getline(cin,html);
	cloIn();system("del tmp.txt > nul ");
			
	const string fSign="%22name%22%3A%22",bSign="%22%2C%22avatar%22%";
	bk=(len=html.length())-18;
	while(++fn<len-15){
		matched=1;
		for(int i=0;i<=15&&matched;i++)if(html[fn+i]!=fSign[i])matched=0;
		if(matched){fn+=16;break;}
	}
	if(!matched)return 0;
	while(--bk){
		matched=1;
		for(int i=0;i<=18&&matched;i++)if(html[bk+i]!=bSign[i])matched=0;
		if(matched){bk-=1;break;}
	}
	userName="\0";
	for(int i=fn;i<=bk;i++)userName+=html[i];
	return 1;
}
int keyMaker(){
	string GIDS=GID,UIDS=UID;
	int charSum=0;
	for(int i=0;i<GIDS.length();i++){
		if(GID[i]=='-')continue;
		charSum=(int)(GIDS[i]+UIDS[i]);
	}
	return charSum;
}
void findCookie(){ 	
	int key=keyMaker(),pos=-1,tmp;
	freopen("cookie.txt","r",stdin);
	scanf("%s",_uid);
	while(true){
		tmp=0;scanf("%d",&tmp);
		if(!tmp)break;
		__client_id[++pos]=(char)(tmp xor key);
	}
	cloIn();
	if(checkCookie())localSave=1,loginAble=1;
	else system("del cookie.txt > nul ");
}
void manageCookie(){
	while(true){
		topbar("Cookie管理");
		if(!loginAble)printf("没有可用的身份验证Cookie.\n你可以从浏览器或有关文件夹获取Cookie.\n"); 
		else {
			cout<<"可用的Luogu账户名:"<<userName<<endl;
			cout<<"Cookie已保存到计算机本地:";
			localSave?printf("是\n"):printf("否\n");
		}
		line();
		if(loginAble)printf("按键 1:更新Cookie\n");
		else printf("按键 1:键入Cookie\n"); 
		if(!localSave)printf("按键 2:将Cookie保存到计算机\n");
		else printf("按键 2:将保存的Cookie从计算机中删除\n");
		printf("按键 其他:退出Cookie管理\n");
		choose=_getch()-'0';
		if(choose==1){
			topbar("Cookie管理");
			printf("在下方键入程序要求的Cookie.\n");
			printf("Cookie本地保存状态已重置.\n");
			line();
			memset(_uid,0,sizeof _uid);memset(__client_id,0,sizeof __client_id);
			loginAble=0;localSave=0;system("del cookie.txt > nul ");
			printf("_uid=");scanf("%s",_uid);
			printf("__client_id=");scanf("%s",__client_id);
			topbar("Cookie管理");
			printf("正在验证你所键入的Cookie...");
			if(!checkCookie()){
				topbar("Cookie管理");
				printf("你键入的Cookie未能被验证.\n");
				printf("检查你的键入,或查看Cookie是否已经过期.\n");
				line();
				printf("按键 所有:返回\n");
				_getch();
				continue; 
			}
			
			loginAble=1;
			topbar("Cookie管理");
			printf("你键入的Cookie已被成功验证.\n");
			cout<<"这个Cookie的用户名为:"<<userName<<endl; 
			line();
			printf("按键 所有:返回\n");
			_getch();
			
		}else if(choose==2){
			if(localSave){
				 topbar("Cookie管理");
				 printf("该操作将删除存储在电脑上的Cookie信息.\n");
				 printf("此时,你仍然可以正常登录,但在关闭程序后将无法登录.\n");
				 line();
				 printf("按键 1:确认删除Cookie信息\n");
				 printf("按键 其他:取消操作\n");
				 choose=_getch()-'0';
				 if(choose==1){
			    	 system("del cookie.txt > nul ");
			    	 localSave=0;
				 }
			}else{
				 topbar("Cookie管理");
				 if(!loginAble){
				 	printf("没有可供保存的Cookie.\n");
				 	printf("请先键入Cookie,若验证通过则可以保存.\n");
				 	line();
				 	printf("按键 所有:返回\n");
				 	_getch();
				 	continue;
				 }
				 printf("该操作将会把你所键入的Cookie写入计算机中.\n");
				 printf("这使得你下次启动程序时无需再键入Cookie.\n");
				 line();
				 printf("按键 1:确认保存Cookie信息\n");
				 printf("按键 其他:取消操作\n");
				 choose=_getch()-'0';
				 if(choose==1){
			    	 freopen("cookie.txt","w",stdout);
					 printf("%s\n",_uid); 
					 int key=keyMaker();
					 for(int i=0;i<strlen(__client_id);i++)printf("%d ",(int)((int)(__client_id[i]) xor key));
					 printf("0");
					 cloOut();
			    	 localSave=1;
				 }			 
			}
		}else return;		
	}
	
}
void saveSetting(){
	freopen("setting.txt","w",stdout);
	for(int i=0;i<=3;i++)printf("%d ",order[i]);printf("\n");
	for(int i=0;i<=2;i++)printf("%d ",fileName[i]);printf("\n");
	printf("%d %d",timLim,delayTim);
	cloOut();
}
void findSetting(){
	freopen("setting.txt","r",stdin);
	for(int i=0;i<=3;i++)scanf("%d ",&order[i]);
	for(int i=0;i<=2;i++)scanf("%d ",&fileName[i]);
	scanf("%d %d",&timLim,&delayTim);
	cloIn();
	if(!delayTim){
		for(int i=0;i<=3;i++)order[i]=i+1;
		timLim=5;delayTim=50;
		saveSetting();
	}
}
void setting(){
	int lev=1,pos=1,sub;
	while(1){
		topbar("选项");
		if(lev==1){
			pos==1?printf("→"):printf("  ");printf("调整保存判定顺序\n");
			pos==2?printf("→"):printf("  ");printf("设置保存时文件名\n");
			pos==3?printf("→"):printf("  ");printf("设置网络请求参数\n");
			line();
			printf("按键 1:切换项目\n");
			printf("按键 2:进入选定的项目\n");
			printf("按键 其他:保存并退出\n");
			choose=_getch()-'0';
			if(choose==1){pos++;if(pos==4)pos=1;}
			else if(choose==2){lev++;sub=pos;pos=1;}
			else {saveSetting();return;}
		}else if(lev==2){
			if(sub==1){
				for(int i=1;i<=4;i++){
					if(i==pos-1)printf("↑");
					else if(i==pos)printf("●");
					else if(i==pos+1)printf("↓");
					else printf("  ");
					
					if(order[i-1]==1)printf("时间\n");
					else if(order[i-1]==2)printf("内存\n");
					else if(order[i-1]==3)printf("代码长度\n");
					else if(order[i-1]==4)printf("提交时间(更新的)\n");
					else printf("提交时间(更旧的)\n"); 
				}
				line();
				printf("按键 1:切换项目\n");
				printf("按键 2:将当前项上移\n");
				printf("按键 3:将当前项下移\n");
				if(order[pos-1]==4||order[pos-1]==5){printf("按键 4:更改时间排序规则\n");printf("按键 其他:返回上一级\n");}
				else printf("按键 其他:返回上一级\n");
				
				choose=_getch()-'0';
				if(choose==1){pos++;if(pos==5)pos=1;} 
				else if(choose==2){if(pos>1)swap(order[pos-1],order[pos-2]),pos--;}
				else if(choose==3){if(pos<4)swap(order[pos-1],order[pos]),pos++;}
				else if(choose==4){
					if(order[pos-1]==4||order[pos-1]==5){
						if(order[pos-1]==4)order[pos-1]=5;
						else order[pos-1]=4;
					}else lev--,pos=1;
				}else lev--,pos=1;	
			}else if(sub==2){
				for(int i=0;i<=2;i++){
					pos==i+1?printf("→"):printf("  ");
					fileName[i]?printf("■"):printf("□");
					if(i==0)printf("题目名称\n");
					else if(i==1)printf("提交时间\n");
					else if(i==2)printf("运行号\n"); 
				}
				line();
				printf("按键 1:切换项目\n");
				printf("按键 2:切换当前选定项目\n");
				printf("按键 其他:返回上一级\n");
				choose=_getch()-'0';
				if(choose==1){pos++;if(pos==4)pos=1;}
				else if(choose==2)fileName[pos-1]=!fileName[pos-1];
				else lev--,pos=1;
			}else if(sub==3){
				printf("调整网络请求的参数.\n");
				printf("设置请求持续时间和相邻请求之间的间隔.\n"); 
				line();
				pos==1?printf("■"):printf("□");printf("请求持续时间 %ds",timLim);
				if(timLim<5)printf(" (设置值可能过小)");printf("\n");
				for(int i=3;i<=15;i++){
					if(i<=timLim)printf("●");
					else printf("○");
				}
				printf("\n");line();
				pos==2?printf("■"):printf("□");printf("相邻请求间隔 %dms",delayTim);
				if(delayTim<100)printf(" (设置值可能过小)");printf("\n");
				for(int i=50;i<=650;i+=50){
					if(i<=delayTim)printf("●");
					else printf("○");
				}
				printf("\n");line();
				printf("按键 1:切换项目\n");
				printf("按键 2:增加时间\n");
				printf("按键 3:减少时间\n");
				
				printf("按键 其他:返回上一级\n");
				choose=_getch()-'0';
				if(choose==1){pos++;if(pos==3)pos=1;}
				else if(choose==2){
					if(pos==1){if(timLim<15)timLim++;}
					else if(pos==2){if(delayTim<650)delayTim+=50;}
				}else if(choose==3){
					if(pos==1){if(timLim>3)timLim--;}
					else if(pos==2){if(delayTim>50)delayTim-=50;}					
				}else lev--,pos=1; 
			}
		}
	}
}

void extract(){
	while(1){
		topbar("提取/管理代码");
		if(!loginAble)printf("当前没有可用的Cookie,请前往Cookie管理页面键入.\n");
		else printf("当前Cookie的账户:%s(%s)\n",_uid,userName.c_str());
		printf("操作3,4需要可用的Cookie,操作的对象是属于当前_uid的代码和题目配置文件.\n");
		line(); 
		printf("按键 1:提取指定_uid的已经爬取的代码\n");
		printf("按键 2:清除已经爬取的所有账户的代码\n");
		printf("按键 3:重新爬取指定题号的题目名称\n");
		printf("按键 4:按当前设置的格式重命名所有代码\n");
		printf("按键 其他:返回\n");
		choose=_getch()-'0';
		if(choose==1){
			topbar("提取/管理代码");
			memset(tarUrl,0,sizeof tarUrl);
			printf("请键入你的_uid:");scanf("%s",tarUrl);
			
			sprintf(option,"dir \"code\\%s\" > nul ",tarUrl);
			topbar("提取/管理代码");
			printf("请稍后...\n");
			if(system(option)){
				topbar("提取/管理代码");
				printf("没有找到这个_uid对应的代码.\n");
				line();
				printf("按键 所有:返回\n");
				_getch();
			}else{
				topbar("提取/管理代码");
				printf("请指定代码要提取到的位置.\n");
				printf("你可以键入绝对路径,或将目标文件夹拖放到该窗口.\n");
				printf("代码将被归档为LuoguCode.zip进行保存,如有重名文件则将自动覆盖.\n");
				line();
				sprintf(option,"tar -cf LuoguCode.zip \"code\\%s\" > nul ",tarUrl);system(option);
				memset(tarUrl,0,sizeof tarUrl);
				printf("目标文件夹绝对路径:");scanf("%s",tarUrl);
				sprintf(option,"move /Y LuoguCode.zip %s\\LuoguCode.zip > nul ",tarUrl);system(option);
				topbar("提取/管理代码");
				printf("操作已完成.\n");
				line();
				printf("按键 所有:返回\n");
				_getch();
			}
		}else if(choose==2){
			topbar("提取/管理代码");
			printf("这会清除所有存储在计算机上的已经爬取的代码.\n");
			printf("增量爬取也将同时不再可用.\n");
			printf("请确认你的操作.\n");
			line();
			printf("按键 1:确认删除\n");
			printf("按键 2:返回\n");
			choose=_getch()-'0';
			if(choose==1){
				system("rd /S /Q code > nul ");
				system("rd /S /Q history > nul ");
				topbar("提取/管理代码");
				printf("操作已完成.\n");
				line();
				printf("按键 所有:返回\n");
				_getch();
			}else continue; 
		}else if(choose==3){
			if(!loginAble){
				topbar("提取/管理代码");
				printf("请先键入一组合法的Cookie.\n");
				line();
				printf("按键 所有:返回\n");
				_getch();
				continue;
			}
			string codeNameS,codeNameN="\0",pid,exName="\0";
			int val[8];
			topbar("提取/管理代码");
			printf("请键入题号:");
			memset(tarUrl,0,sizeof tarUrl);scanf("%s",&tarUrl);
			pid=(string)tarUrl;
			topbar("提取/管理代码");
			printf("请稍后...");
			string probName=getProbName((string)tarUrl);
			if(probName.empty()){
				topbar("提取/管理代码");
				printf("键入的题号没有对应的题目.\n");
				printf("请检查你的题号,然后重试.\n");
				line();
				printf("按键 所有:返回\n");
				_getch(); 
			}else{
				sprintf(option,"copy /Y \"history\\%s\\%s.txt\" tmp.txt > nul ",_uid,pid.c_str());optionRet=system(option);
				if(optionRet){
					freopen("tmp.txt","w",stdout);
					printf("%s/n-/n0 0 0 0 0",probName.c_str());
					cloOut();
				}else{
					freopen("tmp.txt","r",stdin);
					getline(cin,codeNameS);codeNameS="\0";getline(cin,codeNameS);
					if(codeNameS=="-"){
						cloIn();
						freopen("tmp.txt","w",stdout);
						printf("%s/n-/n0 0 0 0 0",probName.c_str());
						cloOut();
					}else{
						//1st=name,2nd=subTim,3rd=rid
						for(int i=0;i<8;i++)scanf("%d",&val[i]);//tim,mem,len,subtim,rid
						cloIn();
						codeNameN+=pid;
						if(val[5])codeNameN+=" ",codeNameN+=probName;
						if(val[6])codeNameN+=" ",codeNameN+=timCov(val[3]);
						if(val[7])codeNameN+=" ",codeNameN+=numCov(val[4]);
						for(int i=codeNameS.length()-1;;i--){
							exName+=codeNameS[i];
							if(codeNameS[i]=='.')break;
						}
						reverse(exName.begin(),exName.end());
						codeNameN+=exName;
						freopen("tmp.txt","w",stdout);
						printf("%s\n%s\n",probName.c_str(),codeNameN.c_str());
						for(int i=0;i<8;i++){
							printf("%d ",val[i]);
							if(i==4)printf("\n");
						}
						cloOut();
						sprintf(option,"move /Y \"tmp.txt\" \"history\\%s\\%s.txt\" > nul ",_uid,pid.c_str());system(option);
						sprintf(option,"ren \"code\\%s\\%s\" \"%s\" > nul ",_uid,codeNameS.c_str(),codeNameN.c_str());system(option);
						
					}
				}
				topbar("提取/管理代码");
				printf("新的题目名称:%s\n",probName.c_str());
				if(!codeNameN.empty())printf("新的代码名称:%s\n旧的代码名称:%s\n",codeNameN.c_str(),codeNameS.c_str());
				else printf("本题没有Accepted的代码.\n");
				line();
				printf("名称修改已完成.\n");
				printf("名称格式依照代码保存时所配置的格式.\n");
				line();
				printf("按键 所有:返回\n");
				_getch();
			}
		}else if(choose==4){
			if(!loginAble){
				topbar("提取/管理代码");
				printf("请先键入一组合法的Cookie.\n");
				line();
				printf("按键 所有:返回\n");
				_getch();
				continue;
			}
			topbar("提取/管理代码");
			printf("在下面预览当前的配置\n");
			line();
			if(!(fileName[0]||fileName[1]))printf("每份代码的文件名仅为其所对应的题号\n");
			else{
				printf("除题号外,以下信息也会作为代码文件名的一部分并按序显示:\n");
				if(fileName[0])printf("题目名称 ");
				if(fileName[1])printf("提交时间 "); 
				if(fileName[2])printf("运行号");
				printf("\n"); 
			}
			line();
			printf("按键 1:确认重命名\n");
			printf("按键 其他:退出\n");
			choose=_getch()-'0';
			if(choose!=1)continue;
			topbar("提取/管理代码");
			printf("请稍后...");
			int num,val[5];
			string pidS,probName,codeNameS,codeNameN,exName;
			vector<string> pidL;
			vector<string>().swap(pidL);
			sprintf(option,"dir /b \"history\\%s\" > tmp.txt ",_uid);system(option);
			system("echo END >> tmp.txt");
			freopen("tmp.txt","r",stdin);
			while(1){
				pidS="";cin>>pidS;
				if(pidS=="latestRid.txt")continue;
				if(pidS=="END")break;
				for(int i=pidS.length()-1;;i--){
					if(pidS[i]=='.'){pidS.erase(i);break;}
					pidS.erase(i);
				}
				//pidS.erase(pidS.length()-1);
				pidL.push_back(pidS);
			}
			cloIn(); 
			num=pidL.size();
			for(int ord=0;ord<num;ord++){
				sprintf(option,"copy /Y \"history\\%s\\%s.txt\" tmp.txt > nul ",_uid,pidL[ord].c_str());
				system(option);
				
				string probName="",codeNameO="",codeNameN="",exName="";
				char codeNameX[100],codeNameY[100];
				int val[5];
				freopen("tmp.txt","r",stdin);
				getline(cin,probName);getline(cin,codeNameO);
				for(int i=0;i<5;i++)scanf("%d",&val[i]);
				cloIn();system("del tmp.txt");
				
				if(codeNameO[0]=='-')continue;
				
				codeNameN+=pidL[ord];
				if(fileName[0])codeNameN+=" ",codeNameN+=probName;
				if(fileName[1])codeNameN+=" ",codeNameN+=timCov(val[3]);
				if(fileName[2])codeNameN+=" ",codeNameN+=numCov(val[4]);
				
				for(int i=codeNameO.length()-1;;i--){
					exName+=codeNameO[i];
					if(codeNameO[i]=='.')break;
				}
				for(int i=exName.length()-1;i>=0;i--)codeNameN+=exName[i];
			
				memset(codeNameX,0,sizeof codeNameX);
				memset(codeNameY,0,sizeof codeNameY);
				for(int i=0;i<codeNameO.length();i++)codeNameX[i]=codeNameO[i];
				for(int i=0;i<codeNameN.length();i++)codeNameY[i]=codeNameN[i];
				sprintf(option,"ren \"code\\%s\\%s\" \"%s\"",_uid,codeNameX,codeNameY);system(option);
				
				freopen("tmp.txt","w",stdout);
				cout<<probName<<endl;
				cout<<codeNameN<<endl;
				for(int i=0;i<5;i++)printf("%d ",val[i]);
				printf("\n");
				for(int i=0;i<3;i++)printf("%d ",fileName[i]);
				cloOut();
				sprintf(option,"move /Y \"tmp.txt\" \"history\\%s\\%s.txt\" > nul ",_uid,pidL[ord].c_str());system(option);
			}
			topbar("提取/管理代码");
			printf("重命名已完成.\n");
			printf("题目配置文件已同步修改.\n");
			line();
			printf("按键 所有:返回\n");
			_getch();
		}else return;
	}
	
}
void about(){
	topbar("关于");
	printf("Luogu Submitting Crawler.exe\n");
	printf("软件版本:%s\n",version.c_str());
	printf("发布日期:%s\n",date.c_str());
	printf("使用Dev-C++ 5.9.2编写,TDM-GCC 4.8.1 64-bit Release编译\n");
	printf("Copyright:2023-2025 Journals Junction Hybrid 保留所有权利\n"); 
	line();
	printf("根据GPLv3,授权相关用户在其计算机上使用此软件\n");
	printf("Windows用户名称: %s\n计算机名称: %s\n",cmptUser,cmptName);
	line(); 
	printf("按键 所有:返回首页\n");
	choose=_getch()-'0';
	return; 
}
void homepage(){
	topbar(" ");
	printf("按键 1:开始爬取Accepted代码\n");
	printf("按键 2:管理/提取已爬取的Accepted代码\n");
	printf("按键 3:管理登录Cookie\n");
	printf("按键 4:选项\n");
	printf("按键 5:关于\n"); 
	printf("按键 其他:退出程序\n");
	choose=_getch()-'0';
	if(choose==1){if(!getCode())getCodeFinal();}
	else if(choose==2)extract();
	else if(choose==3)manageCookie();
	else if(choose==4)setting();
	else if(choose==5)about();
	else exit(0);
} 
void init(){	
	sprintf(option,"title Luogu Submitting Crawler %s",version.c_str());system(option);
	freopen("errlog.txt","w",stderr);
	system("chcp 936 > nul ");SetConsoleOutputCP(936);
	printf("Luogu Submitting Crawler 正在启动\n");line();
	printf("正在读取必要数据...\n");
	
	DWORD MAX_LEN=MAX_SYSINFO+1;
	DWORD computername_len=MAX_COMPUTERNAME_LENGTH+1;
    
   	GetUserName(cmptUser,&MAX_LEN);
    GetComputerName(cmptName,&computername_len); 
	
	string sub_key="SOFTWARE\\Microsoft\\Cryptography",name="MachineGuid";
    HKEY hKey;
    DWORD dwType=REG_SZ;DWORD dwLen=MAX_PATH;
    RegOpenKeyA(HKEY_LOCAL_MACHINE,sub_key.c_str(),&hKey);
	RegQueryValueExA(hKey,name.c_str(),0,&dwType,(LPBYTE)GID,&dwLen);
   	system("wmic csproduct get UUID |find /I \"-\" > tmp.txt");
    system("PowerShell -Command \"&{Get-Culture}\" >> tmp.txt"); 
    freopen("tmp.txt","r",stdin);cin>>UID;
    for(int i=1;i<=4;i++)getline(cin,localID);
	cin>>localID>>localName;
	cloIn();system("del tmp.txt > nul ");
	findSetting();
	printf("正在读取和验证保存在计算机上的Cookie...\n");findCookie();
}
int main(){
	init();
	while(1)homepage();
	return 0;
}
