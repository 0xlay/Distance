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
// File: GdiWrapper.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Media.exe
//
//------------------------------------------------------------------------------

#pragma once
#include <Windows.h>

namespace Distance::Media::GdiWrapper
{

    class HDC final
    {
    public:
        HDC(::HDC hdc) : dc_(hdc), wnd_(nullptr), release_(false)
        {}

        HDC(const HDC&) = delete;
        HDC& operator=(const HDC&) = delete;

        HDC(HDC&& other) noexcept
        {
            std::swap(dc_, other.dc_);
            std::swap(wnd_, other.wnd_);
            std::swap(release_, other.release_);
        }

        HDC& operator=(HDC&& other) noexcept
        {
            clear();
            std::swap(dc_, other.dc_);
            std::swap(wnd_, other.wnd_);
            std::swap(release_, other.release_);
            return *this;
        }

        void releaseOnExit(::HWND wnd)
        {
            release_ = true;
        }

        [[nodiscard]] ::HDC dc() const
        {
            return dc_;
        }

        ~HDC()
        {
            clear();
        }

    private:
        void clear()
        {
            if (release_)
            {
                ReleaseDC(wnd_, dc_);
            }
            else
            {
                DeleteDC(dc_);
            }

            dc_ = nullptr;
            wnd_ = nullptr;
            release_ = false;
        }

    private:
        ::HDC dc_;
        ::HWND wnd_;
        bool release_;
    };



    class HBITMAP final
    {
    public:
        HBITMAP(::HBITMAP bitmap) : bitmap_(bitmap)
        {}

        HBITMAP(const HBITMAP&) = delete;
        HBITMAP& operator=(const HBITMAP&) = delete;

        HBITMAP(HBITMAP&& other) noexcept
        {
            std::swap(bitmap_, other.bitmap_);
        }

        HBITMAP& operator=(HBITMAP&& other) noexcept
        {
            DeleteObject(bitmap_);
            bitmap_ = nullptr;
            std::swap(bitmap_, other.bitmap_);
        }

        [[nodiscard]] ::HBITMAP bitmap() const
        {
            return bitmap_;
        }

        ~HBITMAP()
        {
            DeleteObject(bitmap_);
        }
        
    private:
        ::HBITMAP bitmap_;
    };

} // Distance::Media::GdiWrapper
