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
// File: App.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "ClientNetwork.hpp"
#include "App.hpp"

using namespace Distance::Utils;

namespace Distance
{

    ClientApp& ClientApp::getInstance()
    {
        static ClientApp app;
        return app;
    }

    void ClientApp::run()
    {
        LOG_INFO("Adding modules!");
        ModulesManager::invoke().addModules(CreateModules());

        LOG_INFO("Running modules!");
        ModulesManager::invoke().runAll();

        LOG_INFO("Loading settings from storage!");
        auto settings = std::make_shared<Network::ClientSettings>();
        SettingsController::invoke().load(
            std::reinterpret_pointer_cast<Settings>(settings)
        );

        LOG_INFO("Import settings in TCP client");
        Network::Message msg;
        client_.reset(new Network::DefaultTcpClient(std::move(settings)));

        LOG_INFO("Updating status on Running!");
        Service::invoke().update(ServiceStatus::Running);

        LOG_INFO("Set handler and running TCP client!");
        isRunning_ = true;
        while (isRunning_)
        {
            client_->reset();
            client_->setHandler(Network::Handler::Connect, msg);
            client_->run();
        }

        LOG_INFO("TCP client was stopped!");
    }

    void ClientApp::stop()
    {
        isRunning_ = false;
        client_->stop();
    }

} // namespace Distance
