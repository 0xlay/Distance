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
// File: Convert.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Utils.lib
//
//------------------------------------------------------------------------------

#include "Convert.hpp"
#include "codecvt"


namespace Distance::Utils
{

    LARGE_INTEGER IntegerToLargeInteger(long long value)
    {
        LARGE_INTEGER li;

#ifdef INT64_MAX // Does the compiler natively support 64-bit integers?
        li.QuadPart = value;
#else
        result.high_part = value & 0xFFFFFFFF00000000;
        result.low_part = value & 0xFFFFFFFF;
#endif
        return li;
    }

    [[nodiscard]] long long LargeIntegerToInt64(const LARGE_INTEGER& li)
    {
#ifdef INT64_MAX
        return li.QuadPart;
#else
        return (static_cast<long long>(li.HighPart) << 32) | li.LowPart;
#endif
    }

    std::wstring StringToWString(const std::string& str)
    {
#pragma warning(push)
#pragma warning(disable : 4996)
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(str);
#pragma warning(pop)
    }

    std::string WStringToString(const std::wstring& str)
    {
#pragma warning(push)
#pragma warning(disable : 4996)
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes(str);
#pragma warning(pop)
    }

} // Distance::Utils
