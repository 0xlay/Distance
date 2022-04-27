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
// File: Error.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#include "pch.hpp"
#include "Error.hpp"
#include "Convert.hpp"


namespace Distance
{
    ////////////////////////////////////////////////////////////////////////////
    //
    // Private functions declaration
    //
    ////////////////////////////////////////////////////////////////////////////
    
    void MessageBoxImpl(const std::wstring& title,
                        const std::wstring& msg,
                        uint32_t type);

    ////////////////////////////////////////////////////////////////////////////
    //
    // MessageBoxError implementation
    //
    ////////////////////////////////////////////////////////////////////////////
    
    void MessageBoxError(const std::string& title, const std::string& msg)
    {
        MessageBoxImpl(Utils::StringToWString(title),
                       Utils::StringToWString(msg),
                       MB_OK | MB_ICONERROR);
    }

    void MessageBoxError(const std::wstring& title, const std::wstring& msg)
    {
        MessageBoxImpl(title, msg, MB_OK | MB_ICONERROR);
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // MessageBoxInfo implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    void MessageBoxInfo(const std::string& title, const std::string& msg)
    {
        MessageBoxImpl(Utils::StringToWString(title),
                       Utils::StringToWString(msg),
                       MB_OK | MB_ICONINFORMATION);
    }

    void MessageBoxInfo(const std::wstring& title, const std::wstring& msg)
    {
        MessageBoxImpl(title, msg, MB_OK | MB_ICONINFORMATION);
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // Private functions implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    void MessageBoxImpl(const std::wstring& title,
                        const std::wstring& msg,
                        uint32_t type)
    {
        MessageBoxW(GetActiveWindow(), msg.c_str(), title.c_str(), type);
    }

} // Distance