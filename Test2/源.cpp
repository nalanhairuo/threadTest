#include <stdio.h>
#include <process.h>
#include <windows.h>

const int THREAD_NUM = 10;
int gThreadCount;
unsigned int __stdcall ThreadFunction(LPVOID pPM);

int main(void)
{
    gThreadCount = 0;
    HANDLE hThreadHandle[THREAD_NUM];

    for (int i = 0; i < THREAD_NUM; i++)
    {
        hThreadHandle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, &i, CREATE_SUSPENDED, 0);
        ResumeThread(hThreadHandle[i]);
    }

    WaitForMultipleObjects(THREAD_NUM, hThreadHandle, TRUE, INFINITE);

    return 0;
}

unsigned int __stdcall ThreadFunction(LPVOID pPM)
{
    int ithreadID = *(int *)pPM;

    Sleep(50);
    gThreadCount++;
    Sleep(0);

    printf("线程编号为%d  全局资源值为%d\n", ithreadID, gThreadCount);

    return 0;
}