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
// File: PowerManagerCommand.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "PowerManagerCommand.hpp"
#include "ModulesManager.hpp"

namespace Distance::Command
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // The ShutdownPc command
    //
    ////////////////////////////////////////////////////////////////////////////

    ShutdownPc::ShutdownPc()
    {
        pm_ = std::static_pointer_cast<Module::PowerManager>(
            ModulesManager::invoke().getAccess(ModuleType::PowerManager)
        );
    }

    ShutdownPc& ShutdownPc::execute()
    {
        if (!pm_->shutdown())
        {
            throw std::runtime_error("Failed to shutdown pc!");
        }

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // The RebootPc command
    //
    ////////////////////////////////////////////////////////////////////////////

    RebootPc::RebootPc()
    {
        pm_ = std::static_pointer_cast<Module::PowerManager>(
            ModulesManager::invoke().getAccess(ModuleType::PowerManager)
        );
    }

    RebootPc& RebootPc::execute()
    {
        if (!pm_->reboot())
        {
            throw std::runtime_error("Failed to reboot pc!");
        }

        return *this;
    }

} // Distance::Command
