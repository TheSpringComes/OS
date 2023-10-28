#include"ProducerandConsumer.h"
int main()
{
	//打开文件映像 
	HANDLE hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,F_NAME);
    if (hFileMapping==NULL) {
        printf("In Producer Open File Mapping Failed!\n");
        exit(1);
    }
    LPVOID Data = (share*)MapViewOfFile(hFileMapping,FILE_MAP_ALL_ACCESS,0,0,0);
    if (Data==NULL) {
        printf("Map View of File Failed!\n");
        exit(1);
    }
    struct share_memory *pData = reinterpret_cast<struct share_memory *>(Data);
    //打开信号量 
    empty = OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"EMPTY");
    full  = OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"FULL");
    mutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,"MUTEX");
    
    //写操作 
	int pid = GetCurrentProcessId();
	for(int i=0;i<T_PRODUCER;++i){
		srand((pid+i+time(0))*(pid+i));
		Sleep(RandomTime());
		WaitForSingleObject(empty, INFINITE);//P(empty)
		WaitForSingleObject(mutex, INFINITE);//P(mutex)
		GenerateRandProduct(pData->array[pData->ptail%BUFFER_SIZE], i);//将产品字符串写入缓冲区
		printf("PRODUCER %6.d write: %s, at ",pid,pData->array[pData->ptail%BUFFER_SIZE]);PrintCurrentTime();
		pData->ptail++;
		BufferPrint(pData);
		LONG a; 
		ReleaseSemaphore(full, 1, &a);//V(full) 
		ReleaseMutex(mutex);//V(mutex)
	}
	
	//关闭文件映像和Windows句柄 
	UnmapViewOfFile(Data);
    Data = nullptr;
    CloseHandle(hFileMapping);
    CloseHandle(mutex);
    CloseHandle(empty);
    CloseHandle(full);
    pData = nullptr;
    return 0;
}

