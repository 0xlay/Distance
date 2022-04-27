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
// File: Service.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#pragma once
#include <Windows.h>
#include <winsvc.h>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <memory>


namespace Distance
{

    enum class ServiceStatus : uint32_t
    {
        Stopped               = SERVICE_STOPPED,
        StartPending          = SERVICE_START_PENDING,
        StopPending           = SERVICE_STOP_PENDING,
        Running               = SERVICE_RUNNING,
        ContinuePending       = SERVICE_CONTINUE_PENDING,
        PausePending          = SERVICE_PAUSE_PENDING,
        Paused                = SERVICE_PAUSED
    };



    enum class ServiceControl : uint32_t
    {
        Stop                  = SERVICE_ACCEPT_STOP,
        PauseContinue         = SERVICE_ACCEPT_PAUSE_CONTINUE,
        Shutdown              = SERVICE_ACCEPT_SHUTDOWN,
        ParamChange           = SERVICE_ACCEPT_PARAMCHANGE,
        NetBindChange         = SERVICE_ACCEPT_NETBINDCHANGE,
        HardwareProfileChange = SERVICE_ACCEPT_HARDWAREPROFILECHANGE,
        PowerEvent            = SERVICE_ACCEPT_POWEREVENT,
        SessionChange         = SERVICE_ACCEPT_SESSIONCHANGE,
        PreShutdown           = SERVICE_ACCEPT_PRESHUTDOWN,
        TimeChange            = SERVICE_ACCEPT_TIMECHANGE,
        TriggerEvent          = SERVICE_ACCEPT_TRIGGEREVENT,
        UserModeReboot        = 0x00000800 /* SERVICE_ACCEPT_USERMODEREBOOT: Windows 8 and high */
    };

    inline ServiceControl operator|(ServiceControl lhs, ServiceControl rhs)
    {
        return static_cast<ServiceControl>(static_cast<uint32_t>(lhs) + static_cast<uint32_t>(rhs));
    }



    enum class ServiceType : uint32_t
    {
        KernelDriver          = SERVICE_KERNEL_DRIVER,
        FileSystemDriver      = SERVICE_FILE_SYSTEM_DRIVER,
        Win32OwnProcess       = SERVICE_WIN32_OWN_PROCESS,
        Win32ShareProcess     = SERVICE_WIN32_SHARE_PROCESS,
        UserOwnProcess        = SERVICE_USER_OWN_PROCESS,
        UserShareProcess      = SERVICE_USER_SHARE_PROCESS,
        InteractiveProcess    = SERVICE_INTERACTIVE_PROCESS
    };

    inline ServiceType operator|(ServiceType lhs, ServiceType rhs)
    {
        return static_cast<ServiceType>(static_cast<uint32_t>(lhs) + static_cast<uint32_t>(rhs));
    }



    class Service final
    {
        using ServiceMainProc = void (*)(int, wchar_t**);
        using ServiceControlProc = uint32_t (*)(uint32_t, uint32_t, void*, void*);
    public:
        [[nodiscard]] static Service& invoke();

        void create(
            ServiceControl control,
            ServiceType type,
            std::wstring name,
            ServiceMainProc serviceMain
        );

        void update(ServiceStatus newStatus, uint32_t timeout = 0 /* ms */, uint32_t errorCode = 0);
        void registerHandler(ServiceControlProc handler, LPVOID context = nullptr);

        [[nodiscard]] std::wstring_view baseDirectory();

        Service(const Service&) = delete;
        Service& operator=(const Service&) = delete;
        Service(Service&&) = delete;
        Service& operator=(Service&&) = delete;

        ~Service() = default;

    private:
        Service() = default;

    private:
        SERVICE_TABLE_ENTRYW serviceTable_[2]{};
        SERVICE_STATUS status_{};
        SERVICE_STATUS_HANDLE hServiceStatus_{};
        std::wstring basePathToDirectory_;

        inline static std::unique_ptr<Service> service_{};
    };

} // Distance::Service
