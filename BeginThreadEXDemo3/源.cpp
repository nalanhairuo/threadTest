#include <stdio.h>
#include <windows.h>
#include <process.h>

volatile long g_nLoginCount; //��¼����
unsigned int __stdcall ThreadFun(void *pPM); //�̺߳���
const DWORD THREAD_NUM = 50;//�����߳���

unsigned int __stdcall ThreadFun(void *pPM)
{
    Sleep(100); //some work should to do
    g_nLoginCount++;
    Sleep(50);

    return 0;
}

int main(void)
{
    //�ظ�20���Ա�۲���̷߳���ͬһ��Դʱ���µĳ�ͻ
    int num = 20;

    while (num--)
    {
        g_nLoginCount = 0;

        HANDLE  handle[THREAD_NUM];

        for (int i = 0; i < THREAD_NUM; i++)
        {
            handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);
        }

        WaitForMultipleObjects(THREAD_NUM, handle, true, INFINITE);

        printf("��%d���û���¼���¼�����%d\n", THREAD_NUM, g_nLoginCount);
    }

    return 0;
}