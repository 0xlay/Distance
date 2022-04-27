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
// File: App.hpp
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
#include <memory>

#include "DefaultTcpClient.hpp"
#include "ClientSettings.hpp"
#include "ClientHandler.hpp"
#include "ModulesManager.hpp"
#include "Logger.hpp"
#include "tstring.hpp"
#include "Service.hpp"
#include "Constant.hpp"


namespace Distance
{

    class ClientApp final
    {
    public:
        ClientApp(const ClientApp&) = delete;
        ClientApp(ClientApp&&) = delete;
        ClientApp& operator=(const ClientApp&) = delete;
        ClientApp& operator=(ClientApp&&) = delete;

        static ClientApp& getInstance();
        void run();
        void stop();

    private:
        ClientApp() = default;
        ~ClientApp() = default;

    private:
        bool isRunning_{};
        std::shared_ptr<Network::DefaultTcpClient> client_{};
    };

} // namespace Distance
