#include <stdio.h>
#include <process.h>
#include <windows.h>

//�̸߳���
const int THREAD_NUM = 3;

//ѭ������
const int LOOP = 10;

//�����¼�
HANDLE g_hThreadEvent[THREAD_NUM];//���߳�ͬ���¼�
HANDLE g_Semaphore;               //���߳������߳�ͬ��
//volatile int g_Count = 0;
int g_Count = 0;

unsigned int __stdcall ThreadFunction(void *pPM)
{
    int num = *(int*)pPM;
    ReleaseSemaphore(g_Semaphore, 1, NULL); //�ź���++

    for (int i = 0; i < LOOP; i++)
    {
        WaitForSingleObject(g_hThreadEvent[num], INFINITE);
        //InterlockedIncrement((LPLONG)&g_Count);
        g_Count++;

        printf("��%d�� �߳�ID:%3d,�̴߳�ӡ:%c\n ", g_Count, GetCurrentThreadId(), num + 'A');

        SetEvent(g_hThreadEvent[(num + 1) % THREAD_NUM]);
    }

    return 0;
}


int main(void)
{
    int i = 0;
    HANDLE hThreadHandle[THREAD_NUM];
    g_Semaphore = CreateSemaphore(NULL, 0, 1, NULL); //��ǰ0����Դ���������1��ͬʱ��

    for (i = 0; i < THREAD_NUM; i++)
    {
        g_hThreadEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    for (i = 0; i < THREAD_NUM; i++)
    {
        hThreadHandle[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, &i, 0, nullptr);
        WaitForSingleObject(g_Semaphore, INFINITE);
    }

    SetEvent(g_hThreadEvent[0]);

    WaitForMultipleObjects(THREAD_NUM, hThreadHandle, true, INFINITE);

    for (i = 0; i < THREAD_NUM; i++)
    {
        CloseHandle(hThreadHandle[i]);
        CloseHandle(g_hThreadEvent[i]);
    }

    CloseHandle(g_Semaphore);

    return 0;
}


