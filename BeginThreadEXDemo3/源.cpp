#include <stdio.h>
#include <windows.h>
#include <process.h>

volatile long g_nLoginCount; //登录次数
unsigned int __stdcall ThreadFun(void *pPM); //线程函数
const DWORD THREAD_NUM = 50;//启动线程数

unsigned int __stdcall ThreadFun(void *pPM)
{
    Sleep(100); //some work should to do
    g_nLoginCount++;
    Sleep(50);

    return 0;
}

int main(void)
{
    //重复20次以便观察多线程访问同一资源时导致的冲突
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

        printf("有%d个用户登录后记录结果是%d\n", THREAD_NUM, g_nLoginCount);
    }

    return 0;
}