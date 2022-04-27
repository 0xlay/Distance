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
// File: KeEvent.hpp
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
#include <nonpaged_memory.hpp>


class KeEvent final : public NonPagedMemory
{
public:
    KeEvent()
    {
        KeInitializeEvent(&Event_, SynchronizationEvent, FALSE);
    }

    void Set()
    {
        KeSetEvent(&Event_, IO_NO_INCREMENT, FALSE);
    }

    /*
     * @param[in] timeout - if timeout equals -1 then it's infinity
     */
    [[nodiscard]]
    NTSTATUS Wait(INT64 timeout = -1)
    {
        LARGE_INTEGER li;
        li.QuadPart = timeout;

        LARGE_INTEGER* pTimeout;
        timeout == -1 ? pTimeout = nullptr : pTimeout = &li;

        return 
        KeWaitForSingleObject(&Event_,
                              Executive,
                              KernelMode,
                              FALSE,
                              pTimeout);
    }

private:
    KEVENT Event_ {};
};

