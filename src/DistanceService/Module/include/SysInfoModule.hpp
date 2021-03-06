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
// File: SysInfoModule.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#pragma once

#include "Module.hpp"
#include <variant>


namespace Distance::Module
{

    namespace _internal
    {
        struct Ram
        {
            DWORD MemoryLoad;           /*in percents*/
            DWORDLONG TotalMemory;      /*in bytes*/
            DWORDLONG AvailMemory;      /*in bytes*/
        };

        struct Cpu
        {
            std::string architecture;   /*x64, x86, ...*/
            std::string name;           /*full name: Intel(R) Core(TM) i5-3320M CPU @ 2.60 GHz*/
        };
    }

    /*
    * @brief The SysInfo class it is interface for control SysInfo module
    */
    class SysInfo : public IModule
    {
        using ErrorCode = DWORD;

        using RamInfoFunc = std::variant<_internal::Ram, ErrorCode>(*)();
        using CpuInfoFunc = std::variant<_internal::Cpu, ErrorCode>(*)();
        using LocalNameFunc = std::variant<std::string, ErrorCode>(*)();
        using LocalIpFunc = std::variant<std::string, ErrorCode>(*)();

    public:
        ~SysInfo();

        [[nodiscard]] _internal::Ram ram();
        [[nodiscard]] _internal::Cpu cpu();
        [[nodiscard]] std::string localName();
        [[nodiscard]] std::string localIp();

        void run() override;
        void stop() override;
        
    private:
        HMODULE hModule{};

        RamInfoFunc GetRamInfo;
        CpuInfoFunc GetCpuInfo;
        LocalNameFunc GetLocalName;
        LocalIpFunc GetLocalIp;
    };

    struct SysInfoData
    {
        _internal::Ram ram;
        _internal::Cpu cpu;
        std::string localName;
        std::string localIp;
    };

}
