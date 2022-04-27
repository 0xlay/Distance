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
// File: Configurator.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceServiceConfigurator.exe
//
//------------------------------------------------------------------------------

#include "Configurator.hpp"
#include <sstream>
#include <filesystem>
#include <exception>
#include <stdexcept>

namespace Distance
{

    ServiceConfigurator::ServiceConfigurator(std::wstring_view serviceName)
        : hSCM_{}, hService_{}, serviceInfo_{}, serviceName_(serviceName)
    {
        hSCM_ = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
        if (!hSCM_)
        {
            throw std::runtime_error(
                (std::ostringstream() << "Failed to open Service Control Manager. Error code: "
                << ::GetLastError()).str()
            );
        }
    }

    ServiceConfigurator::~ServiceConfigurator()
    {
        CloseHandle(hService_);
        CloseHandle(hSCM_);
    }

    ErrorCode ServiceConfigurator::install()
    {
        if (hService_)
        {
            throw std::runtime_error("Service already installed!");
        }

        std::wostringstream fullPath;
        fullPath << std::filesystem::current_path().wstring()
                 << L"\\" << serviceName_ << L".exe";

        hService_ = CreateServiceW(
            hSCM_,
            serviceName_.c_str(),
            serviceName_.c_str(),
            SERVICE_ALL_ACCESS,
            SERVICE_WIN32_OWN_PROCESS,
            SERVICE_AUTO_START,
            SERVICE_ERROR_NORMAL,
            fullPath.str().c_str(),
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );
        if (!hService_)
        {
            return ::GetLastError();
        }

        return 0;
    }

    ErrorCode ServiceConfigurator::uninstall()
    {
        if (ErrorCode error = openService(); error)
        {
            return error;
        }

        if (!DeleteService(hService_))
        {
            return ::GetLastError();
        }

        return 0;
    }

    ErrorCode ServiceConfigurator::run()
    {
        if (ErrorCode error = openService(); error)
        {
            return error;
        }

        currentStatus();
        if (serviceInfo_.dwCurrentState == SERVICE_RUNNING || \
            serviceInfo_.dwCurrentState == SERVICE_START_PENDING)
        {
            return 0;
        }

        if (!StartServiceW(hService_, 0 /* argc */, nullptr /* argv */))
        {
            return ::GetLastError();
        }

        return 0;
    }

    ErrorCode ServiceConfigurator::stop()
    {
        if (ErrorCode error = openService(); error)
        {
            return error;
        }

        currentStatus();
        if (serviceInfo_.dwCurrentState == SERVICE_STOPPED || \
            serviceInfo_.dwCurrentState == SERVICE_STOP_PENDING)
        {
            return 0;
        }

        if (!ControlService(hService_, SERVICE_CONTROL_STOP, reinterpret_cast<LPSERVICE_STATUS>(&serviceInfo_)))
        {
            return ::GetLastError();
        }

        return 0;
    }

    ErrorCode ServiceConfigurator::openService()
    {
        if (!hService_)
        {
            hService_ = OpenServiceW(hSCM_, serviceName_.c_str(), SERVICE_ALL_ACCESS);
            if (!hService_)
            {
                return ::GetLastError();
            }
        }

        return 0;
    }

    ErrorCode ServiceConfigurator::currentStatus()
    {
        DWORD bytesNeeded = 0;
        if (!QueryServiceStatusEx(
            hService_,
            SC_STATUS_PROCESS_INFO,
            reinterpret_cast<LPBYTE>(&serviceInfo_),
            sizeof(SERVICE_STATUS_PROCESS),
            &bytesNeeded
        ))
        {
            return ::GetLastError();
        }

        return 0;
    }


} // Distance
