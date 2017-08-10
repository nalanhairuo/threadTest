//�̼߳���Ϣ����

#include "stdio.h"
#include "windows.h"
#include "process.h"
#include "tchar.h"

#define START_MSG WM_USER+102
#define WM_USER_MSG WM_USER+103
#define Quit_MSG WM_USER+104

unsigned int __stdcall acqThread(LPVOID pPID)
{
    unsigned int  id = *(unsigned int *)pPID;

    printf("acqThread run �߳�ID��%d\n", GetCurrentThreadId());
    //printf("send message to dataProcessThread �߳�ID��%d\n", id);

    for (int i = 0; i < 5; i++)
    {
        Sleep(1000);
        PostThreadMessage(id, START_MSG, 0, 0);
    }

    for (int i = 0; i < 5; i++)
    {
        Sleep(1000);
        PostThreadMessage(id, WM_USER_MSG, 0, 0);
    }

    PostThreadMessage(id, Quit_MSG, 0, 0);
    return 0;
}

unsigned int __stdcall dataProcessThread(LPVOID pPID)
{
    printf("dataProcessThread�߳��������߳�ID��%d\n", GetCurrentThreadId());
    unsigned int  id = *(unsigned int *)pPID;

    while (1)
    {
        MSG msg;
        GetMessage(&msg, 0, 0, 0);

        switch (msg.message)
        {
            case START_MSG:
                printf("�߳�ID:%d���յ�%d�̷߳��͵Ŀ�ʼ��Ϣ\n", GetCurrentThreadId(), id);
                break;

            case WM_USER_MSG:
                printf("�߳�ID:%d���յ�%d�̷߳��͵��û���Ϣ\n", GetCurrentThreadId(), id);
                break;

            case Quit_MSG:
                printf("�߳�ID:%d���յ�%d�̷߳��͵��˳���Ϣ\n", GetCurrentThreadId(), id);
                exit(0);

            default:
                break;
        }

    }

    return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
    unsigned int id = 0;
    unsigned int acqId = 0;

    HANDLE m_hthreadHandle = (HANDLE)_beginthreadex(NULL, 0, dataProcessThread, &acqId, CREATE_SUSPENDED, &id);
    ResumeThread(m_hthreadHandle);

    HANDLE m_hthread1Handle = (HANDLE)_beginthreadex(NULL, 0, acqThread, &id, CREATE_SUSPENDED, &acqId);
    ResumeThread(m_hthread1Handle);



    WaitForSingleObject(m_hthread1Handle, INFINITE);

    WaitForSingleObject(m_hthreadHandle, INFINITE);

    //system("pause");
    return 0;
}