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
// File: ClientSettings.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "ClientNetwork.hpp"
#include "ClientSettings.hpp"


using namespace boost::json;


namespace Distance::Network
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // ClientSettings implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    ClientSettings::ClientSettings(std::string_view ip, std::uint32_t port)
        : settings_{ ip.data(), port }
    { }

    void ClientSettings::saveToFile(Utils::_internal::SaveFileHelper file)
    {
        
        file.save((std::ostringstream() << value_from(settings_)).str());
    }

    void ClientSettings::loadFromFile(Utils::_internal::LoadFileHelper file)
    {
        std::string json_string;
        file.load(json_string);
        value json = parse(json_string);
        settings_ = value_to<_internal::ClientSettingsPack>(json);
    }

    void ClientSettings::saveToRegistry(Utils::_internal::SaveRegistryHelper registry)
    {
        registry.save(settings_.ip, L"ip");
        registry.save(settings_.port, L"port");
    }

    void ClientSettings::loadFromRegistry(Utils::_internal::LoadRegistryHelper registry)
    {
        registry.load(settings_.ip, L"ip");
        registry.load(settings_.port, L"port");
    }

    void ClientSettings::setIp(std::string_view ip)
    {
        settings_.ip = ip;
    }

    void ClientSettings::setPort(std::uint32_t port)
    {
        settings_.port = port;
    }

    std::string_view ClientSettings::getIp() const
    {
        return settings_.ip;
    }

    std::uint32_t ClientSettings::getPort() const
    {
        return settings_.port;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // Serialization and deserialize for "ClientSettings"
    //
    ////////////////////////////////////////////////////////////////////////////

    namespace _internal
    {

        template <class T>
        void extract(const object& obj, T& t, string_view key)
        {
            t = value_to<T>(obj.at(key));
        }

        //
        // Overloaded for serialization
        //
        void tag_invoke(value_from_tag, value& val, const ClientSettingsPack& setting)
        {
            val =
            {
                { "ip", setting.ip},
                { "port", setting.port }
            };
        }

        //
        // Overloaded for deserialize
        //
        ClientSettingsPack tag_invoke(value_to_tag<ClientSettingsPack>, const value& json)
        {
            ClientSettingsPack settings;
            const object& obj = json.as_object();

            extract(obj, settings.ip, "ip");
            extract(obj, settings.port, "port");

            return settings;
        }

    } // Distance::Network::_internal

} // Distance::Network