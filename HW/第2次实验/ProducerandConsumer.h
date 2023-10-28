#include<windows.h>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<ctime>
#define F_NAME "MyFileMap"
using namespace std;
const int BUFFER_SIZE   = 6;
const int BUFFER_LENGTH = 10;
const int N_PRODUCER	= 4;//数量  
const int N_CONSUMER	= 4;
const int T_PRODUCER	= 8;//次数 
const int T_CONSUMER	= 8;
const int M_RAND_TIME	= 1200;


typedef struct share_memory{
	char array[BUFFER_SIZE][BUFFER_LENGTH];
	int phead;
	int ptail;
	
}share;
HANDLE mutex;
HANDLE empty;
HANDLE full;
inline int  RandomTime();
inline void PrintCurrentTime();
inline void GenerateRandProduct(char*, int);
inline void BufferPrint(share*);
inline void BufferPrint_ContainsOnly(share*);

inline int RandomTime()
{
	return static_cast<int>(rand()%M_RAND_TIME);//M_RAND_TIME毫秒以内的随机时间
}
inline void PrintCurrentTime()
{
	SYSTEMTIME current_time;
	GetLocalTime(&current_time);
	printf("%04d-%02d-%02d %02d:%02d:%02d.%03d(时:分:秒.毫秒) ", current_time.wYear, current_time.wMonth, current_time.wDay, current_time.wHour, current_time.wMinute, current_time.wSecond, current_time.wMilliseconds);
}
inline void GenerateRandProduct(char a[], int k)
{
	a[0]='P';a[1]='r';a[2]='o';a[3]=k+'0';a[4]='_';
	for(int i=5;i<BUFFER_LENGTH-1;++i){
		a[i]=rand()%26+'a';
	}//不要超过10个，因为最后有个'\0'别忘了 
}
inline void BufferPrint(share* pData)
{
	printf("Now the buffer contains: ");
	if(pData->phead==pData->ptail){
		for(int j=1;j<=6;++j)
			printf("_________ ");
	}
	else if(pData->phead%BUFFER_SIZE <= (pData->ptail-1)%BUFFER_SIZE){
		for(int j=(pData->phead/BUFFER_SIZE)*BUFFER_SIZE;j<(pData->phead/BUFFER_SIZE+1)*BUFFER_SIZE;++j){
			if(j>=pData->phead&&j<pData->ptail)
				printf("%s ", pData->array[j%BUFFER_SIZE]);
			else 
				printf("_________ ");
		}
	}
	else if(pData->phead%BUFFER_SIZE > (pData->ptail-1)%BUFFER_SIZE){
		for(int j=((pData->ptail-1)/BUFFER_SIZE)*BUFFER_SIZE;j<pData->phead+BUFFER_SIZE;++j){
			if(j<pData->ptail)
				printf("%s ", pData->array[j%BUFFER_SIZE]);
			else printf("_________ ");
		}
		for(int j=pData->phead;j<(pData->phead/BUFFER_SIZE+1)*BUFFER_SIZE;++j)
			printf("%s ", pData->array[j%BUFFER_SIZE]);
	}
	printf("\n");
}
inline void BufferPrint_ContainsOnly(share* pData)
{
	if(pData->phead==pData->ptail){
		printf("Now The Buffer Is Empty    \n");
		return;
	}
	printf("Now The Buffer Contains: ");
	for(int i=pData->phead;i<pData->ptail;++i){
		printf("%s ", pData->array[i%BUFFER_SIZE]);
	}
	printf("\n");
}
