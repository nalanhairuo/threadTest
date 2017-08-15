#include <stdio.h>
#include <process.h>
#include <windows.h>

//线程个数
const int THREAD_NUM = 3;

//循环次数
const int LOOP = 10;

//互斥事件
HANDLE g_hThreadEvent[THREAD_NUM];//子线程同步事件
HANDLE g_Semaphore;               //主线程与子线程同步
//volatile int g_Count = 0;
int g_Count = 0;

unsigned int __stdcall ThreadFunction(void *pPM)
{
    int num = *(int*)pPM;
    ReleaseSemaphore(g_Semaphore, 1, NULL); //信号量++

    for (int i = 0; i < LOOP; i++)
    {
        WaitForSingleObject(g_hThreadEvent[num], INFINITE);
        //InterlockedIncrement((LPLONG)&g_Count);
        g_Count++;

        printf("第%d次 线程ID:%3d,线程打印:%c\n ", g_Count, GetCurrentThreadId(), num + 'A');

        SetEvent(g_hThreadEvent[(num + 1) % THREAD_NUM]);
    }

    return 0;
}


int main(void)
{
    int i = 0;
    HANDLE hThreadHandle[THREAD_NUM];
    g_Semaphore = CreateSemaphore(NULL, 0, 1, NULL); //当前0个资源，最大允许1个同时访

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


