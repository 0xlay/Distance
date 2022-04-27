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
// File: Logger.hpp
// 
// Creator: 0xlay
// 
// Environment: Kernel mode only
//
// Module: Dkflt.sys
//
//------------------------------------------------------------------------------

#pragma once
#include <wdm.h>
#include "nonpaged_memory.hpp"

class Logger final : public NonPagedMemory
{
public:

    //
    // Constructors
    //

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

    //
    // Destructor
    //

    ~Logger();

    //
    // Public operators
    //

    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    //
    // Public methods
    //

    [[nodiscard]]
    NTSTATUS Open(LPCWSTR Path);
    void Close();

    [[nodiscard]]
    NTSTATUS Write(LPCWSTR Data);

    [[nodiscard]]
    bool IsOpen() const;

//
// Private data
//

private:
    bool isOpen_;
    HANDLE File_;
    LARGE_INTEGER bytesOffset_;
};
