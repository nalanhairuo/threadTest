//创建多子个线程实例

#include <stdio.h>
#include <windows.h>
#include <process.h>

unsigned int __stdcall ThreadFun(LPVOID p)
{
    printf("ID 号为%d的子线程说hello world\n", GetCurrentThreadId());
    return 0;
}

int main(void)
{
    const int ThreadNum = 10;
    HANDLE ThreadHandle[ThreadNum];

    for (int i = 0; i < ThreadNum; i++)
    {
        ThreadHandle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);
    }

    WaitForMultipleObjects(ThreadNum, ThreadHandle, true, INFINITE);

    return 0;
}