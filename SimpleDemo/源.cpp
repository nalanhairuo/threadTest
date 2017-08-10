#include <stdio.h>
#include <windows.h>
#include <process.h>

unsigned int __stdcall Thread1(LPVOID p)
{
    printf("子线程中使用getcurrentthreadid返回ID号为：%d\n", GetCurrentThreadId());
    return 0;

}
int main()
{
    unsigned int id = 0;
    //HANDLE handle1 = CreateThread(NULL, 0, Thread1, NULL, CREATE_SUSPENDED, &id);
    HANDLE handle1 = (HANDLE)_beginthreadex(NULL, 0, Thread1, NULL, CREATE_SUSPENDED, &id);
    ResumeThread(handle1);
    WaitForSingleObject(handle1, INFINITE);
    printf("主线程中返回子线程的ID号为：%d\n", id);

    return 0;
}