#include "Netfilter.h"


NETFILTER_INFO g_NetFilterInfo;
LONG volatile g_Counter = 0;
FAST_MUTEX g_Mutex;


_Use_decl_annotations_
NTSTATUS
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS status = STATUS_SUCCESS;
    PDEVICE_OBJECT NetfilterObject;

    DriverObject->DriverUnload = NetfilterUnload;

    status = IoCreateDevice(
        DriverObject,
        sizeof(DRIVER_EXTENSION),
        NULL, /*Empty Name*/
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &NetfilterObject
    );
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed create device. Code: 0x%08x\n", status));
        return status;
    }

    status = InitWfp(NetfilterObject);
    if (!NT_SUCCESS(status))
    {
        UnInitWfp(NetfilterObject);
        return  status;
    }


    PDEVICE_EXTENSION deviceExtension = (PDEVICE_EXTENSION)NetfilterObject->DeviceExtension;
    deviceExtension->offset.QuadPart = 0;


    status = GetFullNtPath(NETFILTER_LOGFILE_NAME, &deviceExtension->NtPath, RegistryPath);
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed execute GetFullNtPath. Code: 0x%08x\n", status));
        UnInitWfp((NetfilterObject));
        return status;
    }


    IO_STATUS_BLOCK ioStatusBlock;
    OBJECT_ATTRIBUTES obj;
    InitializeObjectAttributes(
        &obj,
        &deviceExtension->NtPath,
        OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
    )

    status = ZwCreateFile(
        &deviceExtension->file,
        GENERIC_ALL,
        &obj,
        &ioStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        FILE_OVERWRITE_IF,
        FILE_NON_DIRECTORY_FILE,
        NULL,
        0
    );
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed create file. Code: 0x%08x\n", status));
        UnInitWfp((NetfilterObject));
        ExFreePoolWithTag(deviceExtension->NtPath.Buffer, _POOL_TAG_);
        return status;
    }
    ZwClose(deviceExtension->file);


    status = InitWorkItem(NetfilterObject);
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed InitWorkItem. Code: 0x%08x\n", status));
        UnInitWfp((NetfilterObject));
        ExFreePoolWithTag(deviceExtension->NtPath.Buffer, _POOL_TAG_);
        return status;
    }

    ExInitializeFastMutex(&g_Mutex);

    return status;
}



_Use_decl_annotations_
VOID
NetfilterUnload(PDRIVER_OBJECT DriverObject)
{
    PAGED_CODE();
    PDEVICE_EXTENSION deviceExtension = (PDEVICE_EXTENSION)DriverObject->DeviceObject->DeviceExtension;
    UnInitWfp(DriverObject->DeviceObject);

    if (deviceExtension->NtPath.Buffer != NULL)
        ExFreePoolWithTag(deviceExtension->NtPath.Buffer, _POOL_TAG_);

    if (deviceExtension->file != NULL)
        ZwClose(deviceExtension->file);

    IoDeleteDevice(DriverObject->DeviceObject);
}



VOID
ConvertToIPV4(_In_ UINT32 ip, _Inout_ PIPV4 ipv4)
{
    UINT8* bytes = (UINT8*)(&ip);
    ipv4->octet0 = 0xFF & bytes[0];
    ipv4->octet1 = 0xFF & bytes[1];
    ipv4->octet2 = 0xFF & bytes[2];
    ipv4->octet3 = 0xFF & bytes[3];
}



VOID
NTAPI
FilteringRoutine(
    _In_    const FWPS_INCOMING_VALUES0* inFixedValues,
    _In_    const FWPS_INCOMING_METADATA_VALUES0* inMetaValues,
    _Inout_ void* layerData,
    _In_    const FWPS_FILTER0* filter,
    _In_    UINT64 flowContext,
    _Inout_ FWPS_CLASSIFY_OUT0* classifyOut
)
{
    UNREFERENCED_PARAMETER(filter);
    UNREFERENCED_PARAMETER(flowContext);
    UNREFERENCED_PARAMETER(layerData);

    if (FWPS_LAYER_ALE_AUTH_CONNECT_V4 == inFixedValues->layerId)
    {
        const UINT32 IpAddrSource = inFixedValues->incomingValue[
            FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_LOCAL_ADDRESS
        ].value.uint32;

        const UINT32 IpAddrDestination = inFixedValues->incomingValue[
            FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_REMOTE_ADDRESS
        ].value.uint32;

 
        ConvertToIPV4(IpAddrSource, &g_NetFilterInfo.ipv4Source);
        KdPrint(("\nSource IPV4: %d.%d.%d.%d\n",
            g_NetFilterInfo.ipv4Source.octet3,
            g_NetFilterInfo.ipv4Source.octet2,
            g_NetFilterInfo.ipv4Source.octet1,
            g_NetFilterInfo.ipv4Source.octet0
        ));


        ConvertToIPV4(IpAddrDestination, &g_NetFilterInfo.ipv4Destination);
        KdPrint(("\nDestination IPV4: %d.%d.%d.%d\n",
            g_NetFilterInfo.ipv4Destination.octet3,
            g_NetFilterInfo.ipv4Destination.octet2,
            g_NetFilterInfo.ipv4Destination.octet1,
            g_NetFilterInfo.ipv4Destination.octet0
        ));


        if (inFixedValues->incomingValue[
            FWPS_FIELD_ALE_AUTH_CONNECT_V4_ALE_APP_ID
        ].value.type == FWP_BYTE_BLOB_TYPE)
        {
            LPWSTR path = (LPWSTR)inFixedValues->incomingValue[
                FWPS_FIELD_ALE_AUTH_CONNECT_V4_ALE_APP_ID
            ].value.byteBlob->data;

            g_NetFilterInfo.pathToApp = path;
            KdPrint(("\nPath to application: %ws\n", path));
        }


        if (FWPS_IS_METADATA_FIELD_PRESENT(inMetaValues, FWPS_METADATA_FIELD_PROCESS_ID))
        {
            g_NetFilterInfo.processId = inMetaValues->processId;
            KdPrint(("Process id: %I64u\n", g_NetFilterInfo.processId));
        }
    }

    // TODO: add linked list

    SaveInformation(g_NetFilterInfo.workItem);

    classifyOut->actionType = FWP_ACTION_NONE;
}



NTSTATUS
NTAPI
NotifyRoutine(
    _In_ FWPS_CALLOUT_NOTIFY_TYPE notifyType,
    _In_ const GUID* filterKey,
    _In_ FWPS_FILTER0* filter
)
{
    UNREFERENCED_PARAMETER(notifyType);
    UNREFERENCED_PARAMETER(filterKey);
    UNREFERENCED_PARAMETER(filter);
    return STATUS_SUCCESS;
}



NTSTATUS
InitWfp(_In_ PDEVICE_OBJECT NetFilterObject)
{
    PDEVICE_EXTENSION deviceExtension = (PDEVICE_EXTENSION)NetFilterObject->DeviceExtension;
    NTSTATUS status = STATUS_SUCCESS;

    FWPS_CALLOUT0 callout = {
        .calloutKey = NETFILTER_ACTION_CALLOUT_SPYING_AND_TERMINATING,
        .flags = 0,
        .classifyFn = (FWPS_CALLOUT_CLASSIFY_FN0)FilteringRoutine,
        .notifyFn = NotifyRoutine,
        .flowDeleteFn = NULL
    };

    status = FwpsCalloutRegister0(NetFilterObject, &callout, &deviceExtension->calloutRegId);
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed register callout. Code: 0x%lx\n", status));
        return status;
    }

    return status;
}



VOID
UnInitWfp(_In_ PDEVICE_OBJECT NetFilterObject)
{
    const PDEVICE_EXTENSION deviceExtension = (PDEVICE_EXTENSION)NetFilterObject->DeviceExtension;

    if (deviceExtension->calloutRegId != 0)
    {
        FwpsCalloutUnregisterById0(deviceExtension->calloutRegId);
    }
}



NTSTATUS
InitWorkItem(_In_ PDEVICE_OBJECT NetfilterObject)
{
    ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);
    NTSTATUS status = STATUS_SUCCESS;

    g_NetFilterInfo.workItem = IoAllocateWorkItem(NetfilterObject);
    if (g_NetFilterInfo.workItem == NULL)
    {
        KdPrint(("IoAllocateWorkItem was failed allocated memory...\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    return status;
}



VOID
SaveInformation(_In_ PIO_WORKITEM workItem)
{
    IoQueueWorkItem(workItem, WriteFileWorkItem, RealTimeWorkQueue, NULL);
}



VOID
WriteFileWorkItem(_In_ PDEVICE_OBJECT NetfilterObject, _In_ PVOID Context)
{
    ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);
    UNREFERENCED_PARAMETER(Context);

    PDEVICE_EXTENSION deviceExtension = (PDEVICE_EXTENSION)NetfilterObject->DeviceExtension;
    IO_STATUS_BLOCK ioStatusBlock;
    NTSTATUS status = STATUS_SUCCESS;


    LPWSTR buf = ExAllocatePoolWithTag(PagedPool, PAGE_SIZE, _POOL_TAG_);
    if (buf == NULL)
    {
        KdPrint((_DRIVER_PREFIX_": failed allocate paged pool...\n"));
        return;
    }
    RtlZeroMemory(buf, PAGE_SIZE);


    status = RtlStringCbPrintfW(
        buf, 
        PAGE_SIZE, 
        L"Source IPV4: %d.%d.%d.%d <-> Destination IPV4: %d.%d.%d.%d\n"
        L"Process id: %I64u\nPath to application: %ws\n\n",
        g_NetFilterInfo.ipv4Source.octet3,
        g_NetFilterInfo.ipv4Source.octet2,
        g_NetFilterInfo.ipv4Source.octet1,
        g_NetFilterInfo.ipv4Source.octet0,

        g_NetFilterInfo.ipv4Destination.octet3,
        g_NetFilterInfo.ipv4Destination.octet2,
        g_NetFilterInfo.ipv4Destination.octet1,
        g_NetFilterInfo.ipv4Destination.octet0,

        g_NetFilterInfo.processId,
        g_NetFilterInfo.pathToApp
    );
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed execute RtlStringCbPrintfA. Code: 0x%08x\n", status));
        goto end;
    }


    OBJECT_ATTRIBUTES obj;
    InitializeObjectAttributes(
        &obj,
        &deviceExtension->NtPath,
        OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
    )

    ULONG createDisposition = FILE_OPEN_IF;
    if (g_Counter == 5)
    {
        InterlockedExchange(&g_Counter, 0);
        createDisposition = FILE_SUPERSEDE;
        deviceExtension->offset.QuadPart = 0;
    }

    status = ZwCreateFile(
        &deviceExtension->file,
        FILE_GENERIC_READ | FILE_GENERIC_WRITE,
        &obj,
        &ioStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        createDisposition,
        FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0
    );
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed openfile file. Code: 0x%08x\n", status));
        goto end;
    }


    ULONG len = (ULONG)wcslen(buf) * sizeof(WCHAR);
    ExAcquireFastMutex(&g_Mutex);

    status = ZwWriteFile(
        deviceExtension->file,
        NULL, NULL, NULL,
        &ioStatusBlock,
        buf,
        len,
        &deviceExtension->offset,
        NULL
    );
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed write to file. Code: 0x%08x\n", status));
        ExReleaseFastMutex(&g_Mutex);
        goto end;
    }
    deviceExtension->offset.QuadPart += len;
    InterlockedIncrement(&g_Counter);

    ExReleaseFastMutex(&g_Mutex);
    end:
    if (deviceExtension->file != NULL)
        ZwClose(deviceExtension->file);
    ExFreePoolWithTag(buf, _POOL_TAG_);
}



NTSTATUS
GetFullNtPath(
    _In_  LPCWSTR fileName, 
    _Out_ PUNICODE_STRING NtPath, 
    _In_  PUNICODE_STRING RegistryPath
)
{
    HANDLE keyReg;
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING value = RTL_CONSTANT_STRING(L"ImagePath");

    OBJECT_ATTRIBUTES obj;
    InitializeObjectAttributes(
        &obj,
        RegistryPath,
        OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
    )


    status = ZwOpenKey(&keyReg, KEY_QUERY_VALUE, &obj);
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed execute ZwOpenKey. Code: 0x%08x\n", status));
        return status;
    }


    PVOID buffer = ExAllocatePoolWithTag(PagedPool, PAGE_SIZE, _POOL_TAG_);
    if (buffer == NULL)
    {
        KdPrint((_DRIVER_PREFIX_": failed allocate paged pool...\n"));
        ZwClose(keyReg);

        return STATUS_INSUFFICIENT_RESOURCES;
    }
    RtlZeroMemory(buffer, PAGE_SIZE);


    ULONG bytes = 0;
    status = ZwQueryValueKey(
        keyReg,
        &value,
        KeyValuePartialInformation,
        buffer,
        PAGE_SIZE,
        &bytes
    );
    if (!NT_SUCCESS(status))
    {
        KdPrint((_DRIVER_PREFIX_": failed execute ZwQueryValueKey. Code: 0x%08x\n", status));
        ZwClose(keyReg);
        ExFreePoolWithTag(buffer, _POOL_TAG_);
        return status;
    }


    WCHAR* path = ExAllocatePoolWithTag(PagedPool, PAGE_SIZE, _POOL_TAG_);
    if (path == NULL)
    {
        KdPrint((_DRIVER_PREFIX_": failed allocate paged pool...\n"));
        ZwClose(keyReg);
        ExFreePoolWithTag(buffer, _POOL_TAG_);
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    RtlZeroMemory(path, PAGE_SIZE);

    memcpy(path, ((KEY_VALUE_PARTIAL_INFORMATION*)buffer)->Data, MAX_PATH);

    size_t i = wcslen(path) - 1;
    for (; i != 0; --i)
    {
        if (path[i] == L'\\')
        {
            ++i;
            break;
        }
    }

    wcscpy(&path[i], fileName);
    path[i + wcslen(fileName) + 1] = L'\0';
    NtPath->Buffer = path;
    NtPath->Length = (USHORT)wcslen(path) * sizeof(wchar_t);
    NtPath->MaximumLength = PAGE_SIZE;


    ZwClose(keyReg);
    ExFreePoolWithTag(buffer, _POOL_TAG_);
    return status;
}
