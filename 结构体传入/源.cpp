/***********************************************************************
* 版权所有 (C)2017,WQH
*
* 文件名称：
* 内容摘要：
* 其它说明：
* 当前版本：
* 作   者：
* 完成日期：
* //TODO //FIXME

***********************************************************************/
//       File Include Section
//***********************************************************************
#include <windows.h>
#include <iostream>

//***********************************************************************
//       Macro Define Section
//***********************************************************************
//#define MACRO_NAME 0x24
//#define MACRO_FUNC ({...})
//#define MAX(a,b) ((a)>(b) ?(a):(b))
//#define TRACE(var,fmt) printf("TRACE: " #var " = " #fmt "\n",var)
//#define Trace(var) printf("TRACE: " #var " =  %d \n",var)

//***********************************************************************
//       Structure Define Section
//***********************************************************************
// typedef struct
// {
//
// }SELF_DEFINE_TYPE;

//***********************************************************************
//       Global Variable Declare Section
//***********************************************************************
//extern int gGlobalVar;


//***********************************************************************
//       File Static Variable Define Section
//***********************************************************************
//static unsigned int x;
//***********************************************************************
//       Prototype Declare Section
//***********************************************************************
//void Function(...);



using namespace std;


#define NAME_LINE   40

//定义线程函数传入参数的结构体
typedef struct __THREAD_DATA
{
    int nMaxNum;
    char strThreadName[NAME_LINE];

    __THREAD_DATA() : nMaxNum(0)
    {
        memset(strThreadName, 0, NAME_LINE * sizeof(char));
    }
} THREAD_DATA;

HANDLE g_hMutex = NULL;     //互斥量

//线程函数
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    THREAD_DATA* pThreadData = (THREAD_DATA*)lpParameter;

    for (int i = 0; i < pThreadData->nMaxNum; ++ i)
    {
        //请求获得一个互斥量锁
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << pThreadData->strThreadName << " --- " << i << endl;
        Sleep(100);
        //释放互斥量锁
        ReleaseMutex(g_hMutex);
    }

    return 0L;
}

int main(void)
{
    //创建一个互斥量
    g_hMutex = CreateMutex(NULL, FALSE, NULL);

    //初始化线程数据
    THREAD_DATA threadData1, threadData2;
    threadData1.nMaxNum = 5;
    strcpy(threadData1.strThreadName, "线程1");
    threadData2.nMaxNum = 10;
    strcpy(threadData2.strThreadName, "线程2");

    //创建第一个子线程
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc, &threadData1, 0, NULL);
    //创建第二个子线程
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadProc, &threadData2, 0, NULL);
    //关闭线程
    CloseHandle(hThread1);
    CloseHandle(hThread2);

    //主线程的执行路径
    for (int i = 0; i < 5; ++ i)
    {
        //请求获得一个互斥量锁
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << "主线程 === " << i << endl;
        Sleep(100);
        //释放互斥量锁
        ReleaseMutex(g_hMutex);
    }

    system("pause");

    return 0;
}
