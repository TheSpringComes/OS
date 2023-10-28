#include"ProducerandConsumer.h"
HANDLE MyCreateMapping()
{
	//创建临时文件映射对象
	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(share),F_NAME);
	if (hMap==NULL||hMap==INVALID_HANDLE_VALUE) {
        printf("Create File Mapping Failed!\n");
        exit(1);
    }
    //文件映射
    LPVOID pFile = (share*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);  
	if (pFile==NULL){
		printf("Map View of File Failed!\n");
		exit(1);
	}
	//共享内存初始化
	ZeroMemory(pFile,sizeof(share));
	UnmapViewOfFile(pFile);
	pFile = nullptr;
	//文件影响打开 
	HANDLE hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,F_NAME);
    if (hFileMapping==NULL) {
        printf("Open File Mapping Failed!\n");
        exit(1);
    }
    LPVOID Data = (share*)MapViewOfFile(hFileMapping,FILE_MAP_ALL_ACCESS,0,0,0);
    if (Data==NULL) {
        printf("Map View of File Failed!\n");
        exit(1);
    }
	struct share_memory *pData = reinterpret_cast<struct share_memory *>(Data); 
	mutex = CreateMutex(NULL,FALSE,"MUTEX");						//创建互斥锁mutex 
	empty = CreateSemaphore(NULL,BUFFER_SIZE,BUFFER_SIZE,"EMPTY");	//信号量：空缓冲区的数量 
	full  = CreateSemaphore(NULL,0,          BUFFER_SIZE,"FULL");	//信号量：满缓冲区的数量
	memset(pData->array,0,BUFFER_SIZE*BUFFER_LENGTH*sizeof(char));	//缓冲区
	pData->phead = 0;
	pData->ptail = 0;
	
    //解除当前地址空间的映射
    UnmapViewOfFile(Data);
	Data = nullptr;
	CloseHandle(hFileMapping);
	return hMap;
}

void MyCreateSubProcess()//创建子进程
{
    char szFilename[MAX_PATH];
    HANDLE lphandles[N_PRODUCER+N_CONSUMER];
    for(int i = 0; i < N_PRODUCER; i++)
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        sprintf(szFilename,"Producer.exe");
        ZeroMemory(&si,sizeof(STARTUPINFO));
        si.cb=sizeof(STARTUPINFO);
        if (!CreateProcess(szFilename,NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)) {
            printf("Create Producer Process Failed\n");
            exit(1);
        }
        lphandles[i]=pi.hProcess;//把进程句柄存下来
    }
    for(int i = N_PRODUCER; i < N_PRODUCER + N_CONSUMER; i++)
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        sprintf(szFilename,"Consumer.exe");
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        if (!CreateProcess(szFilename, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)){
            printf("Create Consumer Process Failed\n");
            exit(1);
        }
        lphandles[i] = pi.hProcess;//把进程句柄存下来
    }
    WaitForMultipleObjects(N_CONSUMER+N_PRODUCER, lphandles, TRUE, INFINITE);
	for(int i = 0; i < N_CONSUMER+N_PRODUCER; i++)
    {
        //WaitForSingleObject(lphandles[i],INFINITE);
        CloseHandle(lphandles[i]);
    }
    
}

int main(int argc,char * argv[])
{
	srand(time(0));
	printf("-----程序开始 ");PrintCurrentTime();printf("-----\n");
	HANDLE hMap = MyCreateMapping();
	MyCreateSubProcess();
    CloseHandle(hMap);
    CloseHandle(empty);
    CloseHandle(full);
    CloseHandle(mutex);
    printf("\n-----程序结束 ");PrintCurrentTime();printf("-----\n\n");
    system("pause");
	return 0;
}
