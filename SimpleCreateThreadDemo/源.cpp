#include <stdio.h>
#include <windows.h>

unsigned long __stdcall ThreadFun(LPVOID p)
{
    printf("���߳���ʹ��getcurrentthreadid����ID��Ϊ��%d\n", GetCurrentThreadId());
    return 0;
}

int main(void)
{
    DWORD threadID = 0;
    HANDLE handle = CreateThread(NULL, 0, ThreadFun, NULL, CREATE_SUSPENDED, &threadID);
    ResumeThread(handle);
    WaitForSingleObject(handle, INFINITE);
    printf("���߳��з������̵߳�ID��Ϊ��%d\n", threadID);
    return 0;
}