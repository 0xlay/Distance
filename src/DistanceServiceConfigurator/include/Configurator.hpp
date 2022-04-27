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
// File: Configurator.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceServiceConfigurator.exe
//
//------------------------------------------------------------------------------

#pragma once
#include <Windows.h>
#include <string>
#include <string_view>


namespace Distance
{
    using ErrorCode = DWORD;
    class ServiceConfigurator final
    {
    public:
        ServiceConfigurator(std::wstring_view serviceName);
        ~ServiceConfigurator();

        [[maybe_unused]] ErrorCode install();
        [[maybe_unused]] ErrorCode uninstall();
        [[maybe_unused]] ErrorCode run();
        [[maybe_unused]] ErrorCode stop();

    private:
        ErrorCode openService();
        ErrorCode currentStatus();

    private:
        SC_HANDLE hSCM_;
        SC_HANDLE hService_;
        SERVICE_STATUS_PROCESS serviceInfo_;
        std::wstring serviceName_;
    };

} // Distance