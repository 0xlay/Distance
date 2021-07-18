#ifndef _DISTANCE_NETFILTER_H_
#define _DISTANCE_NETFILTER_H_

#define NDIS61 1

#include <ntifs.h>
#include <wdm.h>
#include <fwpsk.h>
#include <fwpmk.h>
#include <ntstrsafe.h>


#define _DRIVER_PREFIX_ "NetfilterCalloutDriver"
#define _POOL_TAG_ 'FteN'
#define NETFILTER_LOGFILE_NAME L"logs\\netlog.txt"


// {C6E41CF6-2749-4382-9268-50AC2335CB1F}
static const GUID NETFILTER_ACTION_CALLOUT_SPYING_AND_TERMINATING =
{ 0xc6e41cf6, 0x2749, 0x4382, { 0x92, 0x68, 0x50, 0xac, 0x23, 0x35, 0xcb, 0x1f } };

// {CE52B6AA-F205-4C13-ADE0-A89C306655EE}
static const GUID NETFILTER_SUB_LAYER =
{ 0xce52b6aa, 0xf205, 0x4c13, { 0xad, 0xe0, 0xa8, 0x9c, 0x30, 0x66, 0x55, 0xee } };



typedef struct _IPV4
{
    UINT8 octet0;
    UINT8 octet1;
    UINT8 octet2;
    UINT8 octet3;
} IPV4, * PIPV4;

VOID
ConvertToIPV4(_In_ UINT32 ip, _Inout_ PIPV4 ipv4);


typedef struct _NETFILTER_INFO
{
    IPV4 ipv4Source;
    IPV4 ipv4Destination;
    LPWSTR pathToApp;
    UINT64 processId;
    PIO_WORKITEM workItem;
} NETFILTER_INFO, *PNETFILTER_INFO;



typedef struct _DEVICE_EXTENSION
{
    UINT32 calloutRegId;
    UNICODE_STRING NtPath;
    HANDLE file;
    LARGE_INTEGER offset;
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;



DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD NetfilterUnload;

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, NetfilterUnload)
#endif



VOID
NTAPI
FilteringRoutine(
    _In_    const FWPS_INCOMING_VALUES0* inFixedValues,
    _In_    const FWPS_INCOMING_METADATA_VALUES0* inMetaValues,
    _Inout_ void* layerData,
    _In_    const FWPS_FILTER0* filter,
    _In_    UINT64 flowContext,
    _Inout_ FWPS_CLASSIFY_OUT0* classifyOut
);

NTSTATUS
NTAPI
NotifyRoutine(
    _In_ FWPS_CALLOUT_NOTIFY_TYPE notifyType,
    _In_ const GUID* filterKey,
    _In_ FWPS_FILTER0* filter
);



NTSTATUS
InitWfp(_In_ PDEVICE_OBJECT NetFilterObject);

VOID
UnInitWfp(_In_ PDEVICE_OBJECT NetFilterObject);



NTSTATUS
InitWorkItem(_In_ PDEVICE_OBJECT NetfilterObject);

VOID
SaveInformation(_In_ PIO_WORKITEM workItem);

VOID
WriteFileWorkItem(_In_ PDEVICE_OBJECT NetfilterObject, _In_ PVOID Context);



NTSTATUS
GetFullNtPath(
    _In_  LPCWSTR fileName, 
    _Out_ PUNICODE_STRING NtPath, 
    _In_  PUNICODE_STRING RegistryPath
);

#endif // _DISTANCE_NETFILTER_H_
