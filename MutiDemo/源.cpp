#include <stdio.h>
#include <windows.h>
#include <process.h>

const int ThreadNum = 64;
volatile int gCount = 0;

unsigned int __stdcall ThreadFun(LPVOID p)
{
    Sleep(100);
    //gCount++;
    InterlockedIncrement((LPLONG)&gCount);
    //InterlockedExchangeAdd((LPLONG)&gCount, 1);
    // printf("线程ID号为%4d的子线程说%d\n", GetCurrentThreadId(), gCount);
    Sleep(50);
    return 0;
}

int main(void)
{
    for (int i = 0; i < 100; i++)
    {
        gCount = 0;
        unsigned int threadId[ThreadNum] = { 0 };
        HANDLE threadHandle[ThreadNum];

        for (int i = 0; i < ThreadNum; i++)
        {
            threadHandle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, CREATE_SUSPENDED, &threadId[i]);
            //threadHandle[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun, NULL, CREATE_SUSPENDED, 0);
            ResumeThread(threadHandle[i]);
        }

        WaitForMultipleObjects(ThreadNum, threadHandle, TRUE, INFINITE);

        printf("threadNum = %d,gCount = %d \n", ThreadNum, gCount);
    }


    return 0;
}