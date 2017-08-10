#include <stdio.h>
#include <windows.h>
#include <process.h>

//主线程启动10个子线程并将表示子线程序号的变量地址作为参数传递给子线程。
//子线程接收参数 -> sleep(50) -> 全局变量++ -> sleep(0) -> 输出参数和全局变量。

//要求：

//1．子线程输出的线程序号不能重复。

//2．全局变量的输出必须递增
//事件与临界区 同步与互斥
const int g_nTHREAD_NUM = 10;
int g_nCount = 0;
CRITICAL_SECTION g_csThreadCode;//全局变量互斥 ,全局变量的输出必须递增
HANDLE  g_hThreadEvent;//子线程输出的线程序号不能重复

unsigned int __stdcall ThreadFunction(LPVOID pPM)
{
    int nVar = *(int *)pPM;
    SetEvent(g_hThreadEvent);
    EnterCriticalSection(&g_csThreadCode);
    Sleep(50);
    g_nCount++;
    Sleep(0);
    LeaveCriticalSection(&g_csThreadCode);
    printf("子线程序号%d,全局变量%d\n", nVar, g_nCount);

    return 0;
}

int main(void)
{
    //初始化事件和关键段 自动置位,初始无触发的匿名事件
    g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
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