#include <stdio.h>
#include <windows.h>

unsigned long __stdcall ThreadFun(LPVOID p)
{
    printf("子线程中使用getcurrentthreadid返回ID号为：%d\n", GetCurrentThreadId());
    return 0;
}

int main(void)
{
    DWORD threadID = 0;
    HANDLE handle = CreateThread(NULL, 0, ThreadFun, NULL, CREATE_SUSPENDED, &threadID);
    ResumeThread(handle);
    WaitForSingleObject(handle, INFINITE);
    printf("主线程中返回子线程的ID号为：%d\n", threadID);
    return 0;
}