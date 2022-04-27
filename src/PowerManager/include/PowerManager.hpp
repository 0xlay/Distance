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
// Module: PowerManager.dll
//
//------------------------------------------------------------------------------

#pragma once

#define _POWER_MANAGER_IMPLEMENTATION_
#include "IPowerManager.hpp"


namespace Distance::Module
{

    extern "C" IPowerManager * CreatePowerManager();

    class PowerManager : public IPowerManager
    {
    public:
        PowerManager();
        [[nodiscard]] bool shutdown() override;
        [[nodiscard]] bool reboot() override;

    private:
        void destruct() override;
        void givePrivilege();

    private:
        DWORD errorCode_;
    };

} // Distance::Module
