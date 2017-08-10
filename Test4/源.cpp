#include <stdio.h>
#include <process.h>
#include <windows.h>

const int nThreadNum = 10;
int gNum = 0;
HANDLE  g_hThreadEvent;
CRITICAL_SECTION  g_csThreadCode;

unsigned int __stdcall ThreadFuction(LPVOID pPM)
{
    int nThreadId = *(int *)(pPM);
    SetEvent(g_hThreadEvent); //触发事件
    Sleep(50);
    EnterCriticalSection(&g_csThreadCode);//进入各子线程互斥区域
    gNum++;
    Sleep(0);
    printf("线程编号为%d  全局资源值为%d\n", nThreadId, gNum);
    LeaveCriticalSection(&g_csThreadCode);//离开各子线程互斥区域
    return 0;
}

int main(void)
{
    //关键段初始化
    InitializeCriticalSection(&g_csThreadCode);
    //初始化事件和关键段 自动置位,初始无触发的匿名事件
    g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    HANDLE threadHandle[nThreadNum];

    for (int i = 0; i < nThreadNum; i++)
    {
        threadHandle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFuction, &i, 0, nullptr);
        WaitForSingleObject(g_hThreadEvent, INFINITE); //等待事件被触发
    }

    WaitForMultipleObjects(nThreadNum, threadHandle, true, INFINITE);

    CloseHandle(g_hThreadEvent);
    DeleteCriticalSection(&g_csThreadCode);

    return 0;
}