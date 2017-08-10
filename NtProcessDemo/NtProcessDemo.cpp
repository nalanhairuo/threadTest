#include <windows.h>
#include <winternl.h>
#include <iostream>
#include "tchar.h"

using namespace std;

typedef NTSTATUS(NTAPI* QEURYINFORMATIONPROCESS)
(IN HANDLE ProcessHandle, IN PROCESSINFOCLASS ProcessInformationClass, OUT PVOID ProcessInformation, IN ULONG ProcessInformationLength, OUT PULONG ReturnLength OPTIONAL);

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
        PROCESS_BASIC_INFORMATION pbi;
        ULONG uLength = 0;
        HMODULE hDll = LoadLibrary(_T("C:\\Windows\\System32\\ntdll.dll"));

        if (hDll)
        {
            QEURYINFORMATIONPROCESS QueryInformationProcess = (QEURYINFORMATIONPROCESS)GetProcAddress(hDll, "NtQueryInformationProcess");

            if (QueryInformationProcess)
            {
                NTSTATUS ntStatus = QueryInformationProcess(processInfomation.hProcess, PROCESSINFOCLASS::ProcessBasicInformation, &pbi, sizeof(pbi), &uLength);

                if (NT_SUCCESS(ntStatus))
                {
                    cout << "Process ID(form Process Control Block):\t" << pbi.UniqueProcessId << endl;
                }

                else
                {
                    cout << "can not open Process Control Block!" << endl << "Error Code:\t" << GetLastError() << endl;
                }
            }

            else
            {
                cout << "cannot get NtQueryInformationProcess function!" << endl << "Error Code:\t" << GetLastError() << endl;
            }

            FreeLibrary(hDll);
        }

        else
        {
            cout << "cannot load ntdll.dll!" << endl << "Error Code:\t" << GetLastError() << endl;
        }
    }

    else
    {
        cout << "can not start process!" << endl;
        cout << "Error code:\t" << GetLastError() << endl;
    }

    return 0;
}