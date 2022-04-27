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
// File: SysInfoCommand.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "SysInfoCommand.hpp"
#include "ModulesManager.hpp"

namespace Distance::Command
{

    GetInfo::GetInfo()
    {
        sysInfo_ = std::static_pointer_cast<Module::SysInfo>(
            ModulesManager::invoke().getAccess(ModuleType::SysInfo)
        );
    }

    GetInfo& GetInfo::execute()
    {
        info_.localIp = sysInfo_->localIp();
        info_.localName = sysInfo_->localName();
        info_.cpu = sysInfo_->cpu();
        info_.ram = sysInfo_->ram();

        return *this;
    }

    Module::SysInfoData GetInfo::pickUp() noexcept
    {
        return std::move(info_);
    }

} // Distance::Command
