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
// File: ProcessManagerCommand.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "ProcessManagerCommand.hpp"
#include "ModulesManager.hpp"

namespace Distance::Command
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // The Terminate command
    //
    ////////////////////////////////////////////////////////////////////////////

    Terminate::Terminate(DWORD pid)
        : pid_(pid)
    {
        pm_ = std::static_pointer_cast<Module::ProcessManager>(
            ModulesManager::invoke().getAccess(ModuleType::ProcessManager)
        );
    }

    Terminate& Terminate::execute()
    {
        pm_->terminate(pid_);
        return  *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // The ProcessList command
    //
    ////////////////////////////////////////////////////////////////////////////

    ProcessList::ProcessList()
    {
        pm_ = std::static_pointer_cast<Module::ProcessManager>(
            ModulesManager::invoke().getAccess(ModuleType::ProcessManager)
        );
    }

    ProcessList& ProcessList::execute()
    {
        list_ = pm_->processList();
        return *this;
    }

    Module::ProcessList ProcessList::pickUp() noexcept
    {
        return std::move(list_);
    }

} // Distance::Command