//------------------------------------------------------------------------------
// ╔═══╗╔══╗╔═══╗╔════╗╔═══╗╔═╗─╔╗╔═══╗╔═══╗
// ╚╗╔╗║╚╣─╝║╔═╗║║╔╗╔╗║║╔═╗║║║╚╗║║║╔═╗║║╔══╝
// ─║║║║─║║─║╚══╗╚╝║║╚╝║║─║║║╔╗╚╝║║║─╚╝║╚══╗
// ─║║║║─║║─╚══╗║──║║──║╚═╝║║║╚╗║║║║─╔╗║╔══╝
// ╔╝╚╝║╔╣─╗║╚═╝║──║║──║╔═╗║║║─║║║║╚═╝║║╚══╗
// ╚═══╝╚══╝╚═══╝──╚╝──╚╝─╚╝╚╝─╚═╝╚═══╝╚═══╝
// 
// Copyright Ⓒ 0xlay. All rights reserved.
// 
// This source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
// 
// File: ProcessManager.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: ProcessManager.dll
//
//------------------------------------------------------------------------------

#include "ProcessManager.hpp"


namespace Distance::Module
{

    IProcessManager* CreateProcessManager()
    {
        return new (std::nothrow) ProcessManager();
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // The ProcessManager implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    bool ProcessManager::terminate(DWORD pid)
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (!hProcess)
            return GetLastError();
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
        return 0;
    }

    std::variant<ProcessList, ErrorCode> ProcessManager::processList()
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

    void ProcessManager::destruct()
    {
        this->~ProcessManager();
    }

} // Distance::Module
