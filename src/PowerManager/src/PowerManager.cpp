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
// File: ProcessManager.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: PowerManager.dll
//
//------------------------------------------------------------------------------

#include "PowerManager.hpp"


namespace Distance::Module
{

    IPowerManager* CreatePowerManager()
    {
        return new(std::nothrow) PowerManager();
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // The PowerManager implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    PowerManager::PowerManager()
        : errorCode_(0)
    {
        givePrivilege();
    }

    bool PowerManager::shutdown()
    {
        if (errorCode_)
            return FALSE;

#pragma warning(push)
#pragma warning(disable : 28159)
        return ExitWindowsEx(EWX_SHUTDOWN, SHTDN_REASON_MAJOR_OTHER);
#pragma warning(pop)
    }

    bool PowerManager::reboot()
    {
        if (errorCode_)
            return FALSE;

        return ExitWindowsEx(EWX_REBOOT, SHTDN_REASON_MAJOR_OTHER);
    }

    void PowerManager::destruct()
    {
        this->~PowerManager();
    }

    void PowerManager::givePrivilege()
    {
        TOKEN_PRIVILEGES tokenPrivileges;
        HANDLE processToken;

        if (!OpenProcessToken(
            GetCurrentProcess(),
            TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processToken
        ))
        {
            errorCode_ = GetLastError();
        }

        if (!LookupPrivilegeValue(
            nullptr,
            SE_SHUTDOWN_NAME,
            &tokenPrivileges.Privileges[0].Luid
        ))
        {
            errorCode_ = GetLastError();
        }

        tokenPrivileges.PrivilegeCount = 1;
        tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        if (!AdjustTokenPrivileges(
            processToken,
            FALSE,
            &tokenPrivileges,
            0,
            nullptr,
            nullptr
        ))
        {
            errorCode_ = GetLastError();
        }
    }

} // Distance::Module
