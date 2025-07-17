/*
This is an update check program for Luogu Submitting Crawler
Update Checker Version: 1 Date: 2025/7/17
*/
#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
void reac(string s){
	ifstream fin;ofstream fout;
	fout.open("update.txt",ios::out);
	fout<<s;fout.close();
	exit(0);
}
void core(){
	string html="",version="";
	int len;
	system("del update.txt > nul 2>nul");
	system("del updateTmp.txt > nul 2>nul");
	system("curl -s https://www.lanzouq.com/b00uz6rixg > updateTmp.txt");
	system("echo [Html File End] >> updateTmp.txt");
	ifstream fin;ofstream fout;
	fin.open("updateTmp.txt",ios::in);
	if(!fin.is_open())reac("Fail");
	while(version.empty()){
		html="";getline(fin,html);
		if(html=="[Html File End]")break;len=html.length();
		for(int i=0;i<len;i++)
			if(html[i]=='@'){while(html[++i]!='@')version+=html[i];break;}
	}
	if(version.empty())reac("Fail");
	else reac(version);
}
int main(){
	core();
	return 0;
}