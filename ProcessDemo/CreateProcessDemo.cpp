#include <iostream>
#include <windows.h>
#include <tchar.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInfomation = { 0 };
    BOOL bSuccess = CreateProcess(_T("C:\\Windows\\notepad.exe"), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &startupInfo, &processInfomation);

    if (bSuccess)
    {
        cout << "process started." << endl
             << "process ID:\t"
             << processInfomation.dwProcessId
             << endl;
    }
    else
    {
        cout << "can not start process!" << endl;
        cout << "Error code:\t" << GetLastError() << endl;
    }

    return system("pause");
}