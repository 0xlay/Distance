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
// File: Convert.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Utils.lib
//
//------------------------------------------------------------------------------


#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

namespace Distance::Utils
{

    [[nodiscard]] LARGE_INTEGER IntegerToLargeInteger(long long value);
    [[nodiscard]] long long LargeIntegerToInteger(const LARGE_INTEGER& li);
    [[nodiscard]] std::wstring StringToWString(const std::string& str);
    [[nodiscard]] std::string WStringToString(const std::wstring& str);

} // Distance::Utils
