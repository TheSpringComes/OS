#include<iostream>
#include<windows.h> 
#include<cstdio>
using namespace std;
int main()
{
	//获取系统当前内存设置信息
	SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
	cout<<sysInfo.dwPageSize<<endl; 
    return 0;
}

