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
// File: SysInfo.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: SysInfo.dll
//
//------------------------------------------------------------------------------

#include "SysInfo.hpp"
#include <sstream>

#pragma comment(lib, "Ws2_32")


namespace Distance::SysInfo
{

    std::variant<Ram, ErroCode> GetRamInfo()
    {
        MEMORYSTATUSEX mem{ 0 };
        mem.dwLength = sizeof(mem);

        if (!GlobalMemoryStatusEx(&mem))
        {
            return GetLastError();
        }

        return Ram{
            mem.dwMemoryLoad,
            mem.ullTotalPhys,
            mem.ullAvailPhys
        };
    }



    std::variant<Cpu, ErroCode> GetCpuInfo()
    {
        Cpu cpu;
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        switch (sysInfo.wProcessorArchitecture)
        {
        case PROCESSOR_ARCHITECTURE_AMD64:
            cpu.architecture = "x64";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            cpu.architecture = "ARM";
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            cpu.architecture = "ARM64";
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            cpu.architecture = "IA64";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            cpu.architecture = "x86";
            break;
        default:
            cpu.architecture = "UNKNOWN";
            break;
        }

        HKEY key;
        auto isError = RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)",
            0,
            KEY_QUERY_VALUE,
            &key
        );
        if (isError)
        {
            return GetLastError();
        }

        DWORD bufSize = MAX_PATH;
        char fullCpuName[MAX_PATH + 1]{ 0 };
        isError = RegGetValueA(
            key,
            nullptr,
            "ProcessorNameString",
            RRF_RT_REG_SZ,
            nullptr,
            fullCpuName,
            &bufSize
        );
        if (isError)
        {
            RegCloseKey(key);
            return GetLastError();
        }

        cpu.name = fullCpuName;
        RegCloseKey(key);

        return cpu;
    }



    std::variant<std::string, ErroCode> GetLocalName()
    {
        char localName[MAX_COMPUTERNAME_LENGTH + 2]{ 0 };
        if (gethostname(localName, MAX_COMPUTERNAME_LENGTH + 1))
        {
            return GetLastError();
        }
        return std::string(localName);
    }



    std::variant<std::string, ErroCode> GetLocalIp()
    {
        auto name = GetLocalName();
        if (std::holds_alternative<ErroCode>(name))
            return std::get<ErroCode>(GetLocalName());

        hostent* info = gethostbyname(std::get<std::string>(name).c_str());
        if (!info)
        {
            return GetLastError();
        }


        struct in_addr addr { 0 };
        std::stringstream ipList;

        for (int i = 0; info->h_addr_list[i] != 0; ++i)
        {
            addr.S_un.S_addr = *(u_long*)info->h_addr_list[i];
            ipList << "{" << inet_ntoa(addr) << "} ";
        }

        return ipList.str();
    }


} // Distance::SysInfo
