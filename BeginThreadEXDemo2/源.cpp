#include <stdio.h>
#include <process.h>
#include <windows.h>

volatile long g_nLoginCount; //��¼����
unsigned int __stdcall ThreadFun(void *pPM); //�̺߳���
const int THREAD_NUM = 50; //�����߳���

unsigned int __stdcall ThreadFun(void *pPM)
{
    Sleep(100); //some work should to do
    g_nLoginCount++;
    Sleep(50);
    return 0;
}

int main(void)
{
    g_nLoginCount = 0;

    HANDLE  handle[THREAD_NUM];

    for (int i = 0; i < THREAD_NUM; i++)
    {
        handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);
    }

    WaitForMultipleObjects(THREAD_NUM, handle, true, INFINITE);

    printf("��%d���û���¼���¼�����%d\n", THREAD_NUM, g_nLoginCount);

    return 0;
}