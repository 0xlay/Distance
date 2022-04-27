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
// File: RemoveLock.cpp
// 
// Creator: 0xlay
// 
// Environment: Kernel mode only
//
// Module: Dkflt.sys
//
//------------------------------------------------------------------------------

#include "RemoveLock.hpp"

////////////////////////////////////////////////////////////////////////////////
//
// The RemoveLock implementations
//
////////////////////////////////////////////////////////////////////////////////

RemoveLock::RemoveLock(ULONG tag, ULONG maxMin, ULONG maxCount)
{
    IoInitializeRemoveLock(&lock_, tag, maxMin, maxCount);
}

void RemoveLock::lock(PVOID tag)
{
    IoAcquireRemoveLock(&lock_, tag);
}

void RemoveLock::unlock(PVOID tag)
{
    IoReleaseRemoveLock(&lock_, tag);
}

void RemoveLock::release(PVOID tag)
{
    IoReleaseRemoveLockAndWait(&lock_, tag);
}

////////////////////////////////////////////////////////////////////////////////
//
// The RemoveLockGuard implementations
//
////////////////////////////////////////////////////////////////////////////////

RemoveLockGuard::RemoveLockGuard(RemoveLock& lock, PVOID tag)
    : lock_(lock), tag_(tag)
{
    lock_.lock(tag_);
}

RemoveLockGuard::~RemoveLockGuard() noexcept
{
    lock_.unlock(tag_);
}
