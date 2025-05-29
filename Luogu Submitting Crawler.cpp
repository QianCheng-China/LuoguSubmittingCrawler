#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<io.h>
#include<string>

#define STDIN 0
#define STDOUT 1
#define STDERR 2 
using namespace std;
const int oldstdout=dup(STDOUT),oldstdin=dup(STDIN),oldstderr=dup(STDERR);
void cloOut(){fclose(stdout);freopen("CON","w",stdout);dup2(oldstdout,STDOUT);}
void cloIn(){fclose(stdin);freopen("CON","r",stdin);dup2(oldstdin,STDIN);}

//complete these information with the cookies from your luogu account.
//Do not share these information with others.
char _uid[]="",__client_id[]="";

char tarUrl[1000],option[2000];
int timlim=5;

int order[4]={1,2,3,4};
//1=tim,2=mem,3=len,4=subtim(newer),5=subtim(older) 
struct best{int tim,mem,len,subtim;};
map<string,best> prob;

bool fileName[5]={0,0};
//1st=subTim,2nd=rid

string timCov(time_t timeStamp){
    struct tm *timeinfo=NULL;char buffer[80];
    timeinfo=localtime(&timeStamp);
    strftime(buffer,80,"%Y.%m.%d %H:%M:%S",timeinfo);
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
	if(x==7||x==25)return ".py";
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

void getRID(){
	const string sign[9]={"%22time%22%3A","memory%22%3A","pid%22%3A%22","difficulty%22%3A","sourceCodeLength%22%3A","submitTime%22%3A","language%22%3A","C%22id%22%3A","status%22%3A"};
	string html,pidS,codeNameS,code,codeS;
	char pid[50],codeName[100];
	bool exist=1,matched,save;
	int pos,tim,mem,diff,len,subtim,lang,rid,status,tmp;
	printf("Start...\n");
	system("md Luogu > nul");
	for(int i=1;exist;i++){
		sprintf(tarUrl,"\"https://www.luogu.com.cn/record/list?user=%s&page=%d\"",_uid,i);
		sprintf(option,"curl --connect-timeout %d -s %s --cookie \"_uid=%s;__client_id=%s\" > tmp.txt",timlim,tarUrl,_uid,__client_id);system(option);
		freopen("tmp.txt","r",stdin);exist=0;pos=0;
		for(int i=1;i<=13;i++)html="\0",getline(cin,html);
		cloIn();
		while(html[++pos]){
			matched=1;
			for(int j=0;j<sign[0].length()&&matched;j++)if(sign[0][j]!=html[pos+j])matched=0;
			if(!matched)continue;
			exist=1;pos+=12;
			tim=0,mem=0,diff=0,len=0,subtim=0,lang=0,rid=0,status=0,tmp=-1;
			memset(pid,0,sizeof pid);pidS="\0";
			while(html[++pos]!='%')tim=(tim<<1)+(tim<<3)+(html[pos]&15);
			for(int i=1;i<=8;i++){
				matched=0;
				while(!matched){
					matched=1;
					for(int j=0;j<sign[i].length()&&matched;j++)if(sign[i][j]!=html[pos+j])matched=0;
					if(!matched){
						if(html[++pos]==')')return;
					}else pos=pos+sign[i].length()-1;
				}
				if(i==1)while(html[++pos]!='%')mem=(mem<<1)+(mem<<3)+(html[pos]&15);
				else if(i==2)while(html[++pos]!='%')pid[++tmp]=html[pos],pidS+=html[pos];
				else if(i==3)while(html[++pos]!='%')diff=(diff<<1)+(diff<<3)+(html[pos]&15);
				else if(i==4)while(html[++pos]!='%')len=(len<<1)+(len<<3)+(html[pos]&15);
				else if(i==5)while(html[++pos]!='%')subtim=(subtim<<1)+(subtim<<3)+(html[pos]&15);
				else if(i==6)while(html[++pos]!='%')lang=(lang<<1)+(lang<<3)+(html[pos]&15);
				else if(i==7)while(html[++pos]!='%')rid=(rid<<1)+(rid<<3)+(html[pos]&15);
				else while(html[++pos]!='%')status=(status<<1)+(status<<3)+(html[pos]&15);
			}
			printf("ID:%d Prob:%s Time:%d Memory:%d Diff:%d Length:%d SubmitTime:%d Language:%d Status:%d\n",rid,pid,tim,mem,diff,len,subtim,lang,status);
			save=1;
			for(int i=0;i<=3&&save&&prob[pidS].subtim;i++){
				if(order[i]==1)save=(prob[pidS].tim>=tim);
				else if(order[i]==2)save=(prob[pidS].mem>=mem);
				else if(order[i]==3)save=(prob[pidS].len>=len);
				else if(order[i]==4)save=(prob[pidS].subtim<subtim);
				else save=(prob[pidS].subtim>subtim);
			}
			if((!save)||(status!=12))printf("Code saving skipped.\n");
			else {
				memset(codeName,0,sizeof codeName);codeNameS="\0";codeNameS+=pidS;
				if(fileName[0])codeNameS+=" ",codeNameS+=timCov(subtim);
				if(fileName[1])codeNameS+=" ",codeNameS+=numCov(rid);
				codeNameS+=langID(lang);
				for(int i=0;codeNameS[i];i++)codeName[i]=codeNameS[i];
				printf("Code will be saved as %s\n",codeName);
				
				sprintf(tarUrl,"\"https://www.luogu.com.cn/record/%d\"",rid);
				sprintf(option,"curl --connect-timeout %d -s %s --cookie \"_uid=%s;__client_id=%s\" > tmp.txt",timlim,tarUrl,_uid,__client_id);system(option);
				
				int fn=0,bk=0;codeS="\0";
				const string fSign="sourceCode%22%3A%22",bSign="%22%2C%22time%22%3A";
				freopen("tmp.txt","r",stdin);
				for(int i=1;i<=13;i++)code="\0",getline(cin,code);
				cloIn();
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
				system("del tmp.txt");
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
				sprintf(option,"ren tmp.txt \"%s\"",codeName);system(option);
				sprintf(option,"move \"%s\" Luogu > nul",codeName);system(option);
			}
		}
	}
	system("del tmp.txt");
}
int main(){
	getRID();
	return 0;
}
