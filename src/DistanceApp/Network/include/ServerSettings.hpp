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
// File: ServerSettings.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#pragma once

#include "SettingsController.hpp"


namespace Distance::Network
{

    enum class InternetProtocol
    {
        IPV4, IPV6
    };

    namespace _internal
    {

        /*
        * @brief The ServerSettingsPack struct keeps the configuration that is needed
        * for the server part
        */
        struct ServerSettingsPack
        {
            InternetProtocol protocol{ InternetProtocol::IPV4 };
            std::uint32_t port{ 2021 };
        };

    } // Distance::Network::_internal

    /*
    * @brief The ServerSettings class for working with server part settings
    */
    class ServerSettings : protected Utils::Settings
    {
    public:
        ServerSettings() = default;
        ServerSettings(InternetProtocol protocol, std::uint32_t port)
            : settings_{ protocol, port }
        { }

        void saveToFile(Utils::_internal::SaveFileHelper file) override;
        void loadFromFile(Utils::_internal::LoadFileHelper file) override;

        void saveToRegistry(Utils::_internal::SaveRegistryHelper registry) override;
        void loadFromRegistry(Utils::_internal::LoadRegistryHelper registry) override;

        void setProtocol(InternetProtocol protocol);
        void setPort(std::uint32_t port);
        [[nodiscard]] InternetProtocol getProtocol() const;
        [[nodiscard]] std::uint32_t getPort() const;

    private:
        _internal::ServerSettingsPack settings_;
    };

} // Distance::Network