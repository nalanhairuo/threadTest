//�������Ӹ��߳�ʵ��

#include <stdio.h>
#include <windows.h>
#include <process.h>

const int ThreadNum = 10;

unsigned int __stdcall ThreadFun(LPVOID p)
{
    printf("ID ��Ϊ%d�����߳�˵hello world\n", GetCurrentThreadId());
    return 0;
}

int main(void)
{

    HANDLE ThreadHandle[ThreadNum];

    for (int i = 0; i < ThreadNum; i++)
    {
        ThreadHandle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);
    }

    WaitForMultipleObjects(ThreadNum, ThreadHandle, true, INFINITE);

    return 0;
}