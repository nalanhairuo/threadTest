//�����߳�ͬ����������
#include <stdio.h>
#include <process.h>
#include <windows.h>

long g_nNum; //ȫ����Դ
unsigned int __stdcall Fun(void *pPM); //�̺߳���
const int THREAD_NUM = 10; //���̸߳���

int main()
{
    g_nNum = 0;
    HANDLE  handle[THREAD_NUM];

    int i = 0;

    while (i < THREAD_NUM)
    {
        handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);
        i++;//�����߳̽��յ�����ʱ���߳̿��ܸı������i��ֵ
    }

    //��֤���߳���ȫ�����н���
    WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
    return 0;
}

unsigned int __stdcall Fun(void *pPM)
{
    //���ڴ����߳���Ҫһ���Ŀ����ģ��������̲߳����ܵ�һʱ��ִ�е�����
    int nThreadNum = *(int *)pPM; //���̻߳�ȡ����
    Sleep(50);//some work should to do
    g_nNum++;  //����ȫ����Դ
    Sleep(0);//some work should to do
    printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadNum, g_nNum);
    return 0;
}