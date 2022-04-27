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
// File: DeviceHelper.cpp
// 
// Creator: 0xlay
// 
// Environment: Kernel mode only
//
// Module: Dkflt.sys
//
//------------------------------------------------------------------------------

#include "DeviceHelper.hpp"


#ifdef ALLOC_PRAGMA

#pragma alloc_text(PAGE, GetDeviceTypeToUse)

#endif


_Use_decl_annotations_
ULONG GetDeviceTypeToUse(PDEVICE_OBJECT DeviceObject)
{
    PAGED_CODE()
        PDEVICE_OBJECT highDeviceObject = IoGetAttachedDeviceReference(DeviceObject);

    if (!highDeviceObject)
    {
        return FILE_DEVICE_UNKNOWN;
    }

    ULONG devType = highDeviceObject->DeviceType;

    ObDereferenceObject(highDeviceObject);
    return devType;
}

