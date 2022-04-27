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
// File: Kfilter.hpp
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
#include "RemoveLock.hpp"

////////////////////////////////////////////////////////////////////////////////
//
// The Kfilter
//
////////////////////////////////////////////////////////////////////////////////

class Kfilter final
{
public:
    Kfilter();

    Kfilter(const Kfilter&) = delete;
    Kfilter(Kfilter&&) = delete;

private:
    [[nodiscard]] NTSTATUS Create(PDRIVER_OBJECT DriverObject,
                                  PDEVICE_OBJECT PhysicalDeviceObject);
    void Remove(PVOID Tag);

    friend Kfilter* CreateKfilter(_In_ PDRIVER_OBJECT DriverObject,
                                  _In_ PDEVICE_OBJECT PhysicalDeviceObject);
    friend void RemoveKfilter(_In_ Kfilter* Filter, _In_ PVOID Tag);

public:
    [[nodiscard]] const PDEVICE_OBJECT GetPhysicalDevice() const;
    [[nodiscard]] const PDEVICE_OBJECT GetLowerDevice() const;

    [[nodiscard]] PDEVICE_OBJECT GetPhysicalDevice();
    [[nodiscard]] PDEVICE_OBJECT GetLowerDevice();

    [[nodiscard]] RemoveLock& GetRemoveLock();

    [[nodiscard]] NTSTATUS Attach(PDEVICE_OBJECT Target);
    void Detach();

    [[nodiscard]] NTSTATUS SetFilterRoutine(PIRP Irp,
                                            PIO_COMPLETION_ROUTINE CompletionRoutine,
                                            [[maybe_unused]] PVOID Context = nullptr);

    void Lock(PVOID Tag);
    void Unlock(PVOID Tag);

    Kfilter& operator=(const Kfilter&) = delete;
    Kfilter& operator=(Kfilter&&) = delete;

    void* operator new(size_t size);
    void operator delete(void* ptr);

private:
    PDEVICE_OBJECT FilteringDeviceObject_{};
    PDEVICE_OBJECT PhysicalDeviceObject_{};
    PDEVICE_OBJECT LowerDeviceObject_{};

    RemoveLock Lock_;

    bool isAttach_ = false;
};

////////////////////////////////////////////////////////////////////////////////
//
// The Kfilter helper functions
//
////////////////////////////////////////////////////////////////////////////////

[[nodiscard]]
Kfilter* CreateKfilter(_In_ PDRIVER_OBJECT DriverObject,
                       _In_ PDEVICE_OBJECT PhysicalDeviceObject);

[[nodiscard]]
Kfilter* GetKfilter(_In_ PDEVICE_OBJECT DeviceObject);

void RemoveKfilter(_In_ Kfilter* Filter, _In_ PVOID Tag);
