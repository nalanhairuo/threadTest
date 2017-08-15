//��ʹ�û������������߳������߳�ͬ��
//���߳������߳̽������10��

#include <stdio.h>
#include <process.h>
#include <windows.h>

HANDLE g_hMutex = nullptr;
const int nLoopNum = 10;

unsigned int __stdcall ThreadFunction(LPVOID pPM)
{

    for (int i = 0; i < nLoopNum; i++)
    {
        WaitForSingleObject(g_hMutex, INFINITE);
        printf("threads says hello\n");
        Sleep(100);
        ReleaseMutex(g_hMutex);
    }

    return 0;
}

int main(void)
{

    HANDLE hThreadHandle = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, nullptr, 0, nullptr);
    g_hMutex = CreateMutex(NULL, FALSE, NULL);

    for (int i = 0; i < nLoopNum; i++)
    {
        WaitForSingleObject(g_hMutex, INFINITE);
        printf("Main says hi\n");
        Sleep(100);
        ReleaseMutex(g_hMutex);
    }

    WaitForSingleObject(hThreadHandle, INFINITE);
    CloseHandle(hThreadHandle);
    return 0;
}