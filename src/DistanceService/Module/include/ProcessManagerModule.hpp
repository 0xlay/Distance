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
// File: ProcessManagerModule.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#pragma once

#include "Module.hpp"
#include "IProcessManager.hpp"


namespace Distance::Module
{
    /*
    * @brief The ProcessManager class it is interface for control ProcessManager module
    */
    class ProcessManager : public IModule
    {
    public:
        ~ProcessManager() override;

        void terminate(DWORD pid);
        [[nodiscard]] ProcessList processList();

        void run() override;
        void stop() override;

    private:
        HMODULE hModule{};
        bool isRun_{};
        std::unique_ptr<IProcessManager, _ProcessManagerInternal::Deleter> processManager_;
    };

} // Distance::Module
