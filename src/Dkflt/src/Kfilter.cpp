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
// File: Kfilter.cpp
// 
// Creator: 0xlay
// 
// Environment: Kernel mode only
//
// Module: Dkflt.sys
//
//------------------------------------------------------------------------------

#include "Kfilter.hpp"
#include "DkfltCommon.hpp"
#include "DeviceHelper.hpp"

#include "Trace.h"
#pragma warning(push)
#pragma warning(disable : 4002)
#include "Kfilter.tmh"
#pragma warning(pop)


////////////////////////////////////////////////////////////////////////////////
//
// The Kfilter implementation
//
////////////////////////////////////////////////////////////////////////////////

Kfilter::Kfilter() : Lock_(D_KFILTER_POOL_TAG, 1, 100)
{ }

NTSTATUS Kfilter::Create(PDRIVER_OBJECT DriverObject,
                         PDEVICE_OBJECT PhysicalDeviceObject)
{
    NTSTATUS ntStatus = IoCreateDevice(DriverObject,
                                       sizeof(Kfilter*),
                                       nullptr,
                                       GetDeviceTypeToUse(PhysicalDeviceObject),
                                       FILE_DEVICE_SECURE_OPEN,
                                       FALSE,
                                       &FilteringDeviceObject_);
    if(NT_SUCCESS(ntStatus))
    {
        FilteringDeviceObject_->DeviceExtension = this;
        PhysicalDeviceObject_ = PhysicalDeviceObject;
    }

    return ntStatus;
}

void Kfilter::Remove(PVOID Tag)
{
    Lock_.release(Tag);
    Detach();
    IoDeleteDevice(FilteringDeviceObject_);
}

const PDEVICE_OBJECT Kfilter::GetPhysicalDevice() const
{
    return PhysicalDeviceObject_;
}

const PDEVICE_OBJECT Kfilter::GetLowerDevice() const
{
    return LowerDeviceObject_;
}

PDEVICE_OBJECT Kfilter::GetPhysicalDevice()
{
    return PhysicalDeviceObject_;
}

PDEVICE_OBJECT Kfilter::GetLowerDevice()
{
    return LowerDeviceObject_;
}

RemoveLock& Kfilter::GetRemoveLock()
{
    return Lock_;
}

NTSTATUS Kfilter::Attach(PDEVICE_OBJECT Target)
{
    LowerDeviceObject_ = IoAttachDeviceToDeviceStack(FilteringDeviceObject_, 
                                                     Target);
    if (!LowerDeviceObject_)
    {
        return STATUS_DEVICE_REMOVED;
    }

    FilteringDeviceObject_->Flags |= \
    LowerDeviceObject_->Flags & (DO_DIRECT_IO | DO_BUFFERED_IO | DO_POWER_PAGABLE);

    FilteringDeviceObject_->Flags &= ~DO_DEVICE_INITIALIZING;
    isAttach_ = true;

    return STATUS_SUCCESS;
}

void Kfilter::Detach()
{
    if (LowerDeviceObject_ && isAttach_)
    {
        IoDetachDevice(LowerDeviceObject_);
        isAttach_ = false;
    }
}

NTSTATUS Kfilter::SetFilterRoutine(PIRP Irp,
                                   PIO_COMPLETION_ROUTINE CompletionRoutine,
                                   [[maybe_unused]] PVOID Context)
{
    return IoSetCompletionRoutineEx(FilteringDeviceObject_,
                                    Irp,
                                    CompletionRoutine, 
                                    nullptr, 
                                    TRUE, FALSE, FALSE);
}

void Kfilter::Lock(PVOID Tag)
{
    Lock_.lock(Tag);
}

void Kfilter::Unlock(PVOID Tag)
{
    Lock_.unlock(Tag);
}

void* Kfilter::operator new(size_t size)
{
    return static_cast<Kfilter*>(
        ExAllocatePoolWithTag(NonPagedPool, size, D_KFILTER_POOL_TAG)
    );
}

void Kfilter::operator delete(void* ptr)
{
    ExFreePoolWithTag(ptr, D_KFILTER_POOL_TAG);
}

////////////////////////////////////////////////////////////////////////////////
//
// The Kfilter helper functions
//
////////////////////////////////////////////////////////////////////////////////

_Use_decl_annotations_
Kfilter* CreateKfilter(PDRIVER_OBJECT DriverObject,
                       PDEVICE_OBJECT PhysicalDeviceObject)
{
    auto filter = new Kfilter();
    if(filter)
    {
        auto status = filter->Create(DriverObject, PhysicalDeviceObject);
        if (!NT_SUCCESS(status))
        {
            filter = nullptr;
        }
    }
    return filter;
}

_Use_decl_annotations_
Kfilter* GetKfilter(PDEVICE_OBJECT DeviceObject)
{
    return static_cast<Kfilter*>(DeviceObject->DeviceExtension);
}

_Use_decl_annotations_
void RemoveKfilter(Kfilter* Filter, PVOID Tag)
{
    Filter->Remove(Tag);
    delete Filter;
}
