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
// File: Logger.cpp
// 
// Creator: 0xlay
// 
// Environment: Kernel mode only
//
// Module: Dkflt.sys
//
//------------------------------------------------------------------------------

#include "Logger.hpp"

Logger::Logger()
    : isOpen_(false), File_(nullptr), bytesOffset_{}
{ }

Logger::~Logger()
{
    if (isOpen_)
    {
        Close();
    }
}

NTSTATUS Logger::Open(LPCWSTR Path)
{
    if (isOpen_)
    {
        Close();
    }

    OBJECT_ATTRIBUTES obj{};
    UNICODE_STRING path{};
    RtlInitUnicodeString(&path, Path);
    InitializeObjectAttributes(&obj,
                               &path,
                               OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                               nullptr,
                               nullptr);

    IO_STATUS_BLOCK ioStatus{};
    NTSTATUS status = ZwCreateFile(&File_,
                                   GENERIC_WRITE | GENERIC_READ,
                                   &obj,
                                   &ioStatus,
                                   nullptr,
                                   FILE_ATTRIBUTE_HIDDEN,
                                   FILE_SHARE_READ,
                                   FILE_OVERWRITE_IF,
                                   FILE_NON_DIRECTORY_FILE,
                                   nullptr,
                                   0);
    if (NT_SUCCESS(status))
    {
        isOpen_ = true;
    }

    return status;
}

void Logger::Close()
{
    if (isOpen_)
    {
        ZwClose(File_);
        isOpen_ = false;
    }
}

NTSTATUS Logger::Write(LPCWSTR Data)
{
    IO_STATUS_BLOCK ioStatus{};

    NTSTATUS status = ZwWriteFile(File_,
                                  nullptr  /* Handle event */,
                                  nullptr, /* APC Routine */
                                  nullptr, /* APC Context */
                                  &ioStatus,
                                  (PVOID)Data,
                                  (ULONG)(wcslen(Data) * sizeof(wchar_t)),
                                  &bytesOffset_,
                                  nullptr);

    bytesOffset_.QuadPart += ioStatus.Information;
    return status;
}

bool Logger::IsOpen() const
{
    return isOpen_;
}
