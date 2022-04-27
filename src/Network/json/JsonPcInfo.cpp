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
// File: JsonPcInfo.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Network.lib
//
//------------------------------------------------------------------------------

#include "Network.hpp"
#include "Json.hpp"

using namespace boost::json;


namespace Distance::Network::Json
{

    template <>
    std::string pack(const PcInfo& object)
    {
        return serialize(value_from(object));
    }

    template <>
    void unpack(const std::string& jsonString, PcInfo& info)
    {
        value jsonValue = parse(jsonString);
        info = value_to<PcInfo>(jsonValue);
    }

    // Serialize PcInfo
    void tag_invoke(value_from_tag, value& jsonValue, const PcInfo& info)
    {
        // Pack PcInfo
        jsonValue =
        {
            { "ram.MemoryLoad", info.ram.MemoryLoad},
            { "ram.TotalMemory", info.ram.TotalMemory},
            { "ram.AvailMemory", info.ram.AvailMemory},

            { "cpu.architecture", info.cpu.architecture },
            { "cpu.name", info.cpu.name },

            { "localName", info.localName},
            { "ip", info.ip}
        };
    }

    // Deserialize PcInfo
    PcInfo tag_invoke(value_to_tag<PcInfo>, const value& jsonValue)
    {
        PcInfo info{};
        const object& obj = jsonValue.as_object();

        //
        // Unpack PcInfo
        //
        info.ram.MemoryLoad = value_to<DWORD>(obj.at("ram.MemoryLoad"));
        info.ram.TotalMemory = value_to<DWORDLONG>(obj.at("ram.TotalMemory"));
        info.ram.AvailMemory = value_to<DWORDLONG>(obj.at("ram.AvailMemory"));

        info.cpu.architecture = value_to<std::string>(obj.at("cpu.architecture"));
        info.cpu.name = value_to<std::string>(obj.at("cpu.name"));

        info.localName = value_to<std::string>(obj.at("localName"));
        info.ip = value_to<std::string>(obj.at("ip"));

        return info;
    }

} // Distance::Network::Json
