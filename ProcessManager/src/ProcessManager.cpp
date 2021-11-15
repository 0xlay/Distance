#include "ProcessManager.hpp"


namespace Distance::ProcessManager
{

    ErrorCode ProcessTerminate(DWORD pid)
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (!hProcess)
            return GetLastError();
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
        return 0;
    }



    std::variant<ProcessList, ErrorCode> GetProcessList()
    {
		PROCESSENTRY32 processEntry{ sizeof(PROCESSENTRY32) };
        ProcessList list;

        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
            return GetLastError();
        if (!Process32First(hSnapshot, &processEntry))
            return GetLastError();

        do
        {
            list.push_back(processEntry);
        } while (Process32Next(hSnapshot, &processEntry));
        
        return list;
    }

} // Distance::ProcessManager
