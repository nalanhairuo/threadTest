#include <stdio.h>
#include <windows.h>
#include <process.h>

//���߳�����10�����̲߳�����ʾ���߳���ŵı�����ַ��Ϊ�������ݸ����̡߳�
//���߳̽��ղ��� -> sleep(50) -> ȫ�ֱ���++ -> sleep(0) -> ���������ȫ�ֱ�����

//Ҫ��

//1�����߳�������߳���Ų����ظ���

//2��ȫ�ֱ���������������
//�ź������ٽ��� ͬ���뻥��
const int g_nTHREAD_NUM = 10;
int g_nCount = 0;
CRITICAL_SECTION g_csThreadCode;//ȫ�ֱ������� ,ȫ�ֱ���������������
HANDLE  g_hThreadEvent;//���߳�������߳���Ų����ظ�

unsigned int __stdcall ThreadFunction(LPVOID pPM)
{
    int nVar = *(int *)pPM;
    ReleaseSemaphore(g_hThreadEvent, 1, nullptr);
    EnterCriticalSection(&g_csThreadCode);
    Sleep(50);
    g_nCount++;
    Sleep(0);
    LeaveCriticalSection(&g_csThreadCode);
    printf("���߳����%d,ȫ�ֱ���%d\n", nVar, g_nCount);

    return 0;
}

int main(void)
{
    //��ʼ���ź����͹ؼ���
    g_hThreadEvent = CreateSemaphore(NULL, 0, 1, NULL);//��ǰ0����Դ���������1��ͬʱ����
    InitializeCriticalSection(&g_csThreadCode);
    HANDLE hThreadHandle[g_nTHREAD_NUM];

    for (int i = 0; i < g_nTHREAD_NUM; i++)
    {
        hThreadHandle[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, &i, CREATE_SUSPENDED, nullptr);
        ResumeThread(hThreadHandle[i]);
        WaitForSingleObject(g_hThreadEvent, INFINITE);
    }

    WaitForMultipleObjects(g_nTHREAD_NUM, hThreadHandle, true, INFINITE);

    DeleteCriticalSection(&g_csThreadCode);
    CloseHandle(g_hThreadEvent);

    return 0;
}