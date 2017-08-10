#include <stdio.h>
#include <windows.h>
#include <process.h>

int g_num = 0;

unsigned int __stdcall ThreadFunction(LPVOID pPM)
{

    g_num++;
    printf("线程ID:%d的子线程输出%d\n", GetCurrentThreadId(), g_num);
    return 0;
}

int main(void)
{
    const int nThreadNum = 10;
    HANDLE hThreadHandle[nThreadNum];

    for (int i = 0; i < nThreadNum; i++)
    {
        hThreadHandle[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, nullptr, 0, nullptr);
    }

    WaitForMultipleObjects(nThreadNum, hThreadHandle, true, INFINITE);
    return 0;
}