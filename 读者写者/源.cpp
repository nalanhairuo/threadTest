//������д������
#include <stdio.h>
#include <process.h>
#include <windows.h>
//���ÿ���̨�����ɫ
BOOL SetConsoleColor(WORD wAttributes)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConsole == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    return SetConsoleTextAttribute(hConsole, wAttributes);
}
const int READER_NUM = 5;  //���߸���
//�ؼ��κ��¼�
CRITICAL_SECTION g_cs, g_cs_writer_count;
HANDLE g_hEventWriter, g_hEventNoReader;
int g_nReaderCount;
//�����߳��������(��κ�����ʵ��)
void ReaderPrintf(char *pszFormat, ...)
{
    va_list   pArgList;

    va_start(pArgList, pszFormat);
    EnterCriticalSection(&g_cs);
    vfprintf(stdout, pszFormat, pArgList);
    LeaveCriticalSection(&g_cs);
    va_end(pArgList);
}
//�����̺߳���
unsigned int __stdcall ReaderThreadFun(PVOID pM)
{
    ReaderPrintf("     ���Ϊ%d�Ķ��߽���ȴ���...\n", GetCurrentThreadId());
    //�ȴ�д�����
    WaitForSingleObject(g_hEventWriter, INFINITE);

    //���߸�������
    EnterCriticalSection(&g_cs_writer_count);
    g_nReaderCount++;

    if (g_nReaderCount == 1)
    {
        ResetEvent(g_hEventNoReader);
    }

    LeaveCriticalSection(&g_cs_writer_count);

    //��ȡ�ļ�
    ReaderPrintf("���Ϊ%d�Ķ��߿�ʼ��ȡ�ļ�...\n", GetCurrentThreadId());

    Sleep(rand() % 100);

    //�����Ķ�,���߸�����С,��λ����
    ReaderPrintf(" ���Ϊ%d�Ķ��߽�����ȡ�ļ�\n", GetCurrentThreadId());

    //���߸�������
    EnterCriticalSection(&g_cs_writer_count);
    g_nReaderCount--;

    if (g_nReaderCount == 0)
    {
        SetEvent(g_hEventNoReader);
    }

    LeaveCriticalSection(&g_cs_writer_count);

    return 0;
}
//д���߳��������
void WriterPrintf(char *pszStr)
{
    EnterCriticalSection(&g_cs);
    SetConsoleColor(FOREGROUND_GREEN);
    printf("     %s\n", pszStr);
    SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    LeaveCriticalSection(&g_cs);
}
//д���̺߳���
unsigned int __stdcall WriterThreadFun(PVOID pM)
{
    WriterPrintf("д���߳̽���ȴ���...");
    //�ȴ����ļ��Ķ���Ϊ��
    WaitForSingleObject(g_hEventNoReader, INFINITE);
    //���д������д�ļ�
    ResetEvent(g_hEventWriter);

    //д�ļ�
    WriterPrintf("  д�߿�ʼд�ļ�.....");
    Sleep(rand() % 100);
    WriterPrintf("  д�߽���д�ļ�");

    //���д�߽���д�ļ�
    SetEvent(g_hEventWriter);
    return 0;
}
int main()
{
    //��ʼ���¼����ź���
    InitializeCriticalSection(&g_cs);
    InitializeCriticalSection(&g_cs_writer_count);

    //�ֶ���λ,��ʼ�Ѵ���
    g_hEventWriter = CreateEvent(NULL, TRUE, TRUE, NULL);
    g_hEventNoReader = CreateEvent(NULL, FALSE, TRUE, NULL);
    g_nReaderCount = 0;

    int i;
    HANDLE hThread[READER_NUM + 1];

    //���������������߳�
    for (i = 1; i <= 2; i++)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThreadFun, NULL, 0, NULL);
    }

    //����д���߳�
    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, WriterThreadFun, NULL, 0, NULL);
    Sleep(50);

    //��������������߽��
    for (; i <= READER_NUM; i++)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThreadFun, NULL, 0, NULL);
    }

    WaitForMultipleObjects(READER_NUM + 1, hThread, TRUE, INFINITE);

    for (i = 0; i < READER_NUM + 1; i++)
    {
        CloseHandle(hThread[i]);
    }

    //�����¼����ź���
    CloseHandle(g_hEventWriter);
    CloseHandle(g_hEventNoReader);
    DeleteCriticalSection(&g_cs);
    DeleteCriticalSection(&g_cs_writer_count);
    return 0;
}