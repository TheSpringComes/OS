#include<iostream>
#include<windows.h> 
#include<cstdio>
using namespace std;
int main()
{
	//��ȡϵͳ��ǰ�ڴ�������Ϣ
	SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
	cout<<sysInfo.dwPageSize<<endl; 
    return 0;
}

