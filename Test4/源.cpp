#include <stdio.h>
#include <process.h>
#include <windows.h>

const int nThreadNum = 10;
int gNum = 0;
HANDLE  g_hThreadEvent;
CRITICAL_SECTION  g_csThreadCode;

unsigned int __stdcall ThreadFuction(LPVOID pPM)
{
    int nThreadId = *(int *)(pPM);
    SetEvent(g_hThreadEvent); //�����¼�
    Sleep(50);
    EnterCriticalSection(&g_csThreadCode);//��������̻߳�������
    gNum++;
    Sleep(0);
    printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadId, gNum);
    LeaveCriticalSection(&g_csThreadCode);//�뿪�����̻߳�������
    return 0;
}

int main(void)
{
    //�ؼ��γ�ʼ��
    InitializeCriticalSection(&g_csThreadCode);
    //��ʼ���¼��͹ؼ��� �Զ���λ,��ʼ�޴����������¼�
    g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    HANDLE threadHandle[nThreadNum];

    for (int i = 0; i < nThreadNum; i++)
    {
        threadHandle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFuction, &i, 0, nullptr);
        WaitForSingleObject(g_hThreadEvent, INFINITE); //�ȴ��¼�������
    }

    WaitForMultipleObjects(nThreadNum, threadHandle, true, INFINITE);

    CloseHandle(g_hThreadEvent);
    DeleteCriticalSection(&g_csThreadCode);

    return 0;
}