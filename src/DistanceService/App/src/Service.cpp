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
// File: Service.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "Service.hpp"
#include "Registry.hpp"
#include "Constant.hpp"


namespace 
{

    void ThrowServiceException(std::string_view msg)
    {
        std::ostringstream oss;
        oss << msg << " Error code : " << GetLastError() << std::endl;
        throw std::runtime_error(oss.str());
    }

}


namespace Distance
{

    Service& Service::invoke()
    {
        if (!service_)
        {
            service_.reset(new Service());
        }

        return *service_;
    }

    void Service::create(
        ServiceControl control,
        ServiceType type,
        std::wstring name,
        ServiceMainProc serviceMain
    )
    {
        using namespace  Utils;

        //
        // Get image path
        //
        Registry reg(HKEY_LOCAL_MACHINE, RegistryOption::NonVolatile, RegistryAccess::Read);
        if (reg.openKey(Constant::serviceRegPath))
        {
            ::ThrowServiceException("Failed to open registry!");
        }
        if (reg.getData(basePathToDirectory_, L"ImagePath"))
        {
            ::ThrowServiceException("Failed to get data from registry!");
        }

        //
        // Convert image path on path to directory
        //
        size_t it = basePathToDirectory_.find(
            (std::wostringstream() << Constant::serviceName << L".exe").str()
        );
        basePathToDirectory_.resize(it);

        status_.dwServiceType      = static_cast<uint32_t>(type);
        status_.dwControlsAccepted = static_cast<uint32_t>(control);
        status_.dwCheckPoint = 0;

        serviceTable_[0].lpServiceName = name.data();
        serviceTable_[0].lpServiceProc = reinterpret_cast<LPSERVICE_MAIN_FUNCTIONW>(serviceMain);

        //
        // Connect to SCM
        //
        if (!StartServiceCtrlDispatcherW(serviceTable_))
        {
            ::ThrowServiceException("Failed to connect to SCM!");
        }
    }

    void Service::update(ServiceStatus newStatus, uint32_t timeout /* 0ms */, uint32_t errorCode /* 0 */)
    {
        status_.dwCurrentState = static_cast<uint32_t>(newStatus);
        status_.dwCheckPoint++;
        status_.dwWaitHint = timeout;

        if (!errorCode)
        {
            status_.dwWin32ExitCode = NO_ERROR;
        }
        else
        {
            status_.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
            status_.dwServiceSpecificExitCode = errorCode;
        }

        if (!SetServiceStatus(hServiceStatus_, &status_))
        {
            ::ThrowServiceException("Failed to set new service status!");
        }
    }

    void Service::registerHandler(ServiceControlProc handler, LPVOID context)
    {
        hServiceStatus_ = RegisterServiceCtrlHandlerExW(
            serviceTable_[0].lpServiceName,
            reinterpret_cast<LPHANDLER_FUNCTION_EX>(handler),
            context
        );
        if (!hServiceStatus_)
        {
            ::ThrowServiceException("Failed to register service control handler!");
        }
    }

    std::wstring_view Service::baseDirectory()
    {
        return basePathToDirectory_;
    }

} // Distance
