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
    
    //进行读操作 
	int pid = GetCurrentProcessId();  
	for(int i=0;i<T_CONSUMER;++i){
		srand((pid+i)*(pid+i));
		Sleep(RandomTime());
		WaitForSingleObject(full, INFINITE);//P(full)
		WaitForSingleObject(mutex, INFINITE);//P(mutex)
		char tem[10];
		sprintf(tem,pData->array[pData->phead%BUFFER_SIZE]);
		printf("COMSUMER %6.d read : %s, at ",pid,tem);PrintCurrentTime();
		pData->phead++;
		BufferPrint(pData);
		LONG a;
		ReleaseSemaphore(empty, 1, &a);//V(empty)
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

