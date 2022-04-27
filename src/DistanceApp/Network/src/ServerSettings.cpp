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
// File: ServerSettings.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#include "pch.hpp"
#include "ServerSettings.hpp"

using namespace boost::json;


namespace Distance::Network
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // ServerSettings implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    void ServerSettings::saveToFile(Utils::_internal::SaveFileHelper file)
    {
        file.save((std::ostringstream() << value_from(settings_)).str());
    }

    void ServerSettings::loadFromFile(Utils::_internal::LoadFileHelper file)
    {
        std::string json_string;
        file.load(json_string);
        value json = parse(json_string);
        settings_ = value_to<_internal::ServerSettingsPack>(json);
    }

    void ServerSettings::saveToRegistry(Utils::_internal::SaveRegistryHelper registry)
    {
        if (settings_.protocol == InternetProtocol::IPV6)
        {
            registry.save(L"IPV6", L"protocol");
        }
        else
        {
            registry.save(L"IPV4", L"protocol");
        }

        registry.save(settings_.port, L"port");
    }

    void ServerSettings::loadFromRegistry(Utils::_internal::LoadRegistryHelper registry)
    {
        std::wstring protocol;
        registry.load(protocol, L"protocol");
        registry.load(settings_.port, L"port");

        if (protocol == L"IPV6")
        {
            settings_.protocol = InternetProtocol::IPV6;
        }
        else
        {
            settings_.protocol = InternetProtocol::IPV4;
        }
    }

    void ServerSettings::setProtocol(InternetProtocol protocol)
    {
        settings_.protocol = protocol;
    }

    void ServerSettings::setPort(std::uint32_t port)
    {
        settings_.port = port;
    }

    InternetProtocol ServerSettings::getProtocol() const
    {
        return settings_.protocol;
    }

    std::uint32_t ServerSettings::getPort() const
    {
        return settings_.port;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // Serialization and deserialize for "ServerSettings"
    //
    ////////////////////////////////////////////////////////////////////////////

    namespace _internal
    {

        template <class T>
        void extract(const object& obj, T& t, string_view key)
        {
            t = value_to<T>(obj.at(key));
        }

        // Overloaded for serialization
        void tag_invoke(value_from_tag, value& val, const ServerSettingsPack& settings)
        {
            std::string protocol_;
            switch (settings.protocol)
            {
            case InternetProtocol::IPV6:
                protocol_ = "IPV6";
                break;
            case InternetProtocol::IPV4:
            default:
                protocol_ = "IPV4";
                break;
            }

            val =
            {
                { "protocol", std::move(protocol_) },
                { "port", settings.port }
            };
        }

        // Overloaded for deserialize
        ServerSettingsPack tag_invoke(value_to_tag<ServerSettingsPack>, const value& json)
        {
            ServerSettingsPack settings;
            std::string protocol;
            const object& obj = json.as_object();

            extract(obj, protocol, "protocol");
            extract(obj, settings.port, "port");

            if (protocol == "IPV6")
            {
                settings.protocol = InternetProtocol::IPV6;
            }
            else
            {
                settings.protocol = InternetProtocol::IPV4;
            }

            return settings;
        }

    } // Distance::Network::_internal

} // Distance::Network