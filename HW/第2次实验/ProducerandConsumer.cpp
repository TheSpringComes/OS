#include"ProducerandConsumer.h"
HANDLE MyCreateMapping()
{
	//������ʱ�ļ�ӳ�����
	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(share),F_NAME);
	if (hMap==NULL||hMap==INVALID_HANDLE_VALUE) {
        printf("Create File Mapping Failed!\n");
        exit(1);
    }
    //�ļ�ӳ��
    LPVOID pFile = (share*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);  
	if (pFile==NULL){
		printf("Map View of File Failed!\n");
		exit(1);
	}
	//�����ڴ��ʼ��
	ZeroMemory(pFile,sizeof(share));
	UnmapViewOfFile(pFile);
	pFile = nullptr;
	//�ļ�Ӱ��� 
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
	mutex = CreateMutex(NULL,FALSE,"MUTEX");						//����������mutex 
	empty = CreateSemaphore(NULL,BUFFER_SIZE,BUFFER_SIZE,"EMPTY");	//�ź������ջ����������� 
	full  = CreateSemaphore(NULL,0,          BUFFER_SIZE,"FULL");	//�ź�������������������
	memset(pData->array,0,BUFFER_SIZE*BUFFER_LENGTH*sizeof(char));	//������
	pData->phead = 0;
	pData->ptail = 0;
	
    //�����ǰ��ַ�ռ��ӳ��
    UnmapViewOfFile(Data);
	Data = nullptr;
	CloseHandle(hFileMapping);
	return hMap;
}

void MyCreateSubProcess()//�����ӽ���
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
        lphandles[i]=pi.hProcess;//�ѽ��̾��������
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
        lphandles[i] = pi.hProcess;//�ѽ��̾��������
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
	printf("-----����ʼ ");PrintCurrentTime();printf("-----\n");
	HANDLE hMap = MyCreateMapping();
	MyCreateSubProcess();
    CloseHandle(hMap);
    CloseHandle(empty);
    CloseHandle(full);
    CloseHandle(mutex);
    printf("\n-----������� ");PrintCurrentTime();printf("-----\n\n");
    system("pause");
	return 0;
}
