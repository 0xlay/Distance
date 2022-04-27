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
// File: RemoveLock.hpp
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

////////////////////////////////////////////////////////////////////////////////
//
// The RemoveLock
//
////////////////////////////////////////////////////////////////////////////////

class RemoveLock final
{
public:
    RemoveLock(ULONG tag, ULONG maxMin, ULONG maxCount);

    void lock(PVOID tag);
    void unlock(PVOID tag);
    void release(PVOID tag);

private:
    IO_REMOVE_LOCK lock_{};
};

////////////////////////////////////////////////////////////////////////////////
//
// The RemoveLockGuard
//
////////////////////////////////////////////////////////////////////////////////

class RemoveLockGuard final
{
public:
    RemoveLockGuard(RemoveLock& lock, PVOID tag);
    ~RemoveLockGuard() noexcept;

    RemoveLockGuard(const RemoveLockGuard&) = delete;
    RemoveLockGuard(RemoveLockGuard&&) = delete;

    RemoveLockGuard& operator=(const RemoveLockGuard&) = delete;
    RemoveLockGuard& operator=(RemoveLockGuard&&) = delete;

private:
    RemoveLock& lock_;
    PVOID tag_;
};
