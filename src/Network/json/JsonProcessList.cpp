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
// File: JsonProcessList.cpp
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
    std::string pack(const ProcessList& list)
    {
        return serialize(value_from(list));
    }

    template <>
    void unpack(const std::string& jsonString, ProcessList& list)
    {
        value jsonValue = parse(jsonString);
        list = value_to<ProcessList>(jsonValue);
    }

} // Distance::Network::Json

// Serialize ProcessList
void tag_invoke(value_from_tag, value& jsonValue, const PROCESSENTRY32& process)
{
    // Pack PROCESSENTRY32
    jsonValue =
    {
        {"dwSize", process.dwSize},
        {"cntUsage", process.cntUsage},
        {"th32ProcessID", process.th32ProcessID},
        {"th32DefaultHeapID", process.th32DefaultHeapID},
        {"th32ModuleID", process.th32ModuleID},
        {"cntThreads", process.cntThreads},
        {"th32ParentProcessID", process.th32ParentProcessID},
        {"pcPriClassBase", process.pcPriClassBase},
        {"dwFlags", process.dwFlags},
        {"szExeFile", xstar::tstring(process.szExeFile)}
    };
}

// Deserialize ProcessList
PROCESSENTRY32 tag_invoke(value_to_tag<PROCESSENTRY32>, const value& jsonValue)
{
    const object& entry = jsonValue.as_object();

    PROCESSENTRY32 entry32
    {
        value_to<DWORD>(entry.at("dwSize")),
        value_to<DWORD>(entry.at("cntUsage")),
        value_to<DWORD>(entry.at("th32ProcessID")),
        value_to<ULONG_PTR>(entry.at("th32DefaultHeapID")),
        value_to<DWORD>(entry.at("th32ModuleID")),
        value_to<DWORD>(entry.at("cntThreads")),
        value_to<DWORD>(entry.at("th32ParentProcessID")),
        value_to<LONG>(entry.at("pcPriClassBase")),
        value_to<DWORD>(entry.at("dwFlags")),
        0 /*szExeFile*/
    };

    auto exeFile = value_to<xstar::tstring>(entry.at("szExeFile"));
    memcpy(entry32.szExeFile, exeFile.c_str(), exeFile.length() * sizeof(TCHAR));

    return entry32;
}
