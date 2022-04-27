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
// File: PowerManagerModule.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "PowerManagerModule.hpp"

namespace Distance::Module
{

    PowerManager::~PowerManager()
    {
        if (isRun_)
            stop();
    }

    bool PowerManager::shutdown()
    {
        return powerManager_->shutdown();
    }

    bool PowerManager::reboot()
    {
        return powerManager_->reboot();
    }

    void PowerManager::run()
    {
        hModule = LoadLibrary(TEXT("PowerManager.dll"));
        if (!hModule)
            throw std::runtime_error(
                "Failed to load the PowerManager library! Error code: " + std::to_string(GetLastError())
            );

        auto CreatePowerManager = reinterpret_cast<_PowerManagerInternal::PowerManagerAPI>(
            GetProcAddress(hModule, "CreatePowerManager")
        );
        if (!CreatePowerManager)
        {
            throw std::runtime_error("Failed to get a valid procedure address!");
        }
        powerManager_.reset(CreatePowerManager());
        isRun_ = true;
    }

    void PowerManager::stop()
    {
        powerManager_.get_deleter()(powerManager_.release());
        FreeLibrary(hModule);
        isRun_ = false;
    }

} // Distance::Module