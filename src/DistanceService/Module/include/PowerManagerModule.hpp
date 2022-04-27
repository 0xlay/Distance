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
// File: PowerManagerModule.hpp
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
#include "IPowerManager.hpp"


namespace Distance::Module
{
    /*
    * @brief The PowerManager class it is interface for control PowerManager module
    */
    class PowerManager : public IModule
    {
    public:
        ~PowerManager();

        [[nodiscard]] bool shutdown();
        [[nodiscard]] bool reboot();

        void run() override;
        void stop() override;

    private:
        HMODULE hModule{};
        bool isRun_{};
        std::unique_ptr<IPowerManager, _PowerManagerInternal::Deleter> powerManager_;
    };

} // Distance::Module
