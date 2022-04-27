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
// File: IProcessManager.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <variant>


namespace Distance::Module
{
    using ProcessList = std::vector<PROCESSENTRY32>;
    using ErrorCode = DWORD;

#ifndef _PROCESS_MANAGER_IMPLEMENTATION_

    namespace _ProcessManagerInternal
    {

        struct Deleter;

    } // _ProcessManagerInternal

#endif // _PROCESS_MANAGER_IMPLEMENTATION_

    struct IProcessManager
    {
        virtual ~IProcessManager() = default;

        virtual bool terminate(DWORD pid) = 0;
        virtual std::variant<ProcessList, ErrorCode> processList() = 0;

    private:
#ifndef _PROCESS_MANAGER_IMPLEMENTATION_

        friend _ProcessManagerInternal::Deleter;

#endif // _PROCESS_MANAGER_IMPLEMENTATION_

        virtual void destruct() = 0;
    };

#ifndef _PROCESS_MANAGER_IMPLEMENTATION_

    namespace _ProcessManagerInternal
    {

        using ProcessManagerAPI = IProcessManager* (*)(void);

        struct Deleter
        {
            void operator()(IProcessManager* manager)
            {
                manager->destruct();
            }
        };

    } // _ProcessManagerInternal

 #endif // _PROCESS_MANAGER_IMPLEMENTATION_

} // Distance::Module