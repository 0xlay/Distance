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
// File: ClientSettings.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#pragma once

#include "SettingsController.hpp"


namespace Distance::Network
{

    namespace _internal
    {
        /*
        * @brief The "ClientSettingPack" keeps the configuration that is needed
        * for the client part
        */
        struct ClientSettingsPack
        {
            std::string ip{ "127.0.0.1" };
            std::uint32_t port{ 2021 };
        };
    } // Distance::Network::_internal

    /*
    * @brief The "ClientSettings" is the interface for working with client part
    * settings
    */
    class ClientSettings : protected Utils::Settings
    {
    public:
        ClientSettings() = default;
        ClientSettings(std::string_view ip, std::uint32_t port);

        void saveToFile(Utils::_internal::SaveFileHelper file) override;
        void loadFromFile(Utils::_internal::LoadFileHelper file) override;

        void saveToRegistry(Utils::_internal::SaveRegistryHelper registry) override;
        void loadFromRegistry(Utils::_internal::LoadRegistryHelper registry) override;

        void setIp(std::string_view ip);
        void setPort(std::uint32_t port);
        [[nodiscard]] std::string_view getIp() const;
        [[nodiscard]] std::uint32_t getPort() const;

    private:
        _internal::ClientSettingsPack settings_;
    };

} // Distance::Network