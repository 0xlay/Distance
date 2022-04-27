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
// File: ProcessManagerModule.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "ProcessManagerModule.hpp"


namespace Distance::Module
{

    ProcessManager::~ProcessManager()
    {
        if (isRun_)
            stop();
    }

    void ProcessManager::terminate(DWORD pid)
    {
        ErrorCode code = processManager_->terminate(pid);
        if (code)
            throw std::runtime_error("Failed to terminate process! Error code: " + std::to_string(code));
    }

    ProcessList ProcessManager::processList()
    {
        auto list = processManager_->processList();
        if (std::holds_alternative<ErrorCode>(list))
            throw std::runtime_error("Failed to have process list! Error code: " + std::to_string(std::get<ErrorCode>(list)));
        
        return std::get<ProcessList>(list);
    }

    void ProcessManager::run()
    {
        hModule = LoadLibrary(TEXT("ProcessManager.dll"));
        if (!hModule)
            throw std::runtime_error(
                "Failed to load the ProcessManager library! Error code: " + std::to_string(GetLastError())
            );

        auto CreateProcessManager = reinterpret_cast<_ProcessManagerInternal::ProcessManagerAPI>(
            GetProcAddress(hModule, "CreateProcessManager")
        );

        if (!CreateProcessManager)
            throw std::runtime_error("Failed to get a valid procedure address!");

        processManager_.reset(CreateProcessManager());
        isRun_ = true;
    }

    void ProcessManager::stop()
    {
        processManager_.get_deleter()(processManager_.release());
        FreeLibrary(hModule);
        isRun_ = false;
    }

} // Distance::Module