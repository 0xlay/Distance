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
// File: SysInfo.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "SysInfoModule.hpp"


namespace Distance::Module
{

    SysInfo::~SysInfo()
    {
        FreeLibrary(hModule);
    }

    void SysInfo::run()
    {
        hModule = LoadLibrary(TEXT("SysInfo.dll"));
        if (!hModule)
            throw std::runtime_error(
                "Failed to load the PcInfo library! Error code: " + std::to_string(GetLastError())
            );

        GetRamInfo	 = reinterpret_cast<RamInfoFunc>(GetProcAddress(hModule, "GetRamInfo"));
        GetCpuInfo	 = reinterpret_cast<CpuInfoFunc>(GetProcAddress(hModule, "GetCpuInfo"));
        GetLocalName = reinterpret_cast<LocalNameFunc>(GetProcAddress(hModule, "GetLocalName"));
        GetLocalIp	 = reinterpret_cast<LocalIpFunc>(GetProcAddress(hModule, "GetLocalIp"));

        if (!GetRamInfo || !GetCpuInfo || !GetLocalName || !GetLocalIp)
            throw std::runtime_error("Failed to get a valid procedure address!");
    }

    void SysInfo::stop()
    {
        FreeLibrary(hModule);
    }

    _internal::Ram SysInfo::ram()
    {
        auto ramInfo = GetRamInfo();
        if (std::holds_alternative<ErrorCode>(ramInfo))
            throw std::runtime_error(
                "Failed to get a memory info! Error code: " + std::to_string(std::get<ErrorCode>(ramInfo))
            );

        return std::get<_internal::Ram>(ramInfo);
    }

    _internal::Cpu SysInfo::cpu()
    {
        auto cpuInfo = GetCpuInfo();
        if (std::holds_alternative<ErrorCode>(cpuInfo))
            throw std::runtime_error(
                "Failed to get a cpu info! Error code: " + std::to_string(std::get<ErrorCode>(cpuInfo))
            );
        
        return std::get<_internal::Cpu>(cpuInfo);
    }

    std::string SysInfo::localName()
    {
        auto name = GetLocalName();
        if (std::holds_alternative<ErrorCode>(name))
            throw std::runtime_error(
                "Failed to get a local name! Error code: " + std::to_string(std::get<ErrorCode>(name))
            );

        return std::get<std::string>(name);
    }

    std::string SysInfo::localIp()
    {
        auto ip = GetLocalIp();
        if (std::holds_alternative<ErrorCode>(ip))
            throw std::runtime_error(
                "Failed to get a IP! Error code: " + std::to_string(std::get<ErrorCode>(ip))
            );

        return std::get<std::string>(ip);
    }

} // Distance::Module
