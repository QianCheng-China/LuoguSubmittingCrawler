/*
This is a program to get local information for Luogu Submitting Crawler
Local Fetcher Version: 1 Date: 2025/7/17
*/
#define WINVER       0x0600
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;
void getLocal(){
	wchar_t localeName[LOCALE_NAME_MAX_LENGTH]={0};
    GetUserDefaultLocaleName(localeName,LOCALE_NAME_MAX_LENGTH);wcout<<localeName<<endl;
    LCID userLcid=GetUserDefaultLCID();LANGID userLangId=GetUserDefaultLangID();
    wcout<<userLcid<<endl;
    wchar_t country[16]={0};
    GetLocaleInfoW(userLcid,LOCALE_SISO3166CTRYNAME,country,_countof(country));wcout<<country<<endl;
}
int main() {
	freopen("sysinfo.txt","a",stdout);
	getLocal();
	fclose(stdout);freopen("CON","w",stdout);
    return 0;
}
