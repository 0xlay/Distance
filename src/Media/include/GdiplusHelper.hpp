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
// File: GdiplusHelper.hpp
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
#include "GdiPlus.hpp"
#include "Image.hpp"


namespace Distance::Media::GdiplusHelper
{

    [[nodiscard]] int GetEncoderClsid(const wchar_t* format, CLSID* clsid);

    class GdiPlus final
    {
    public:
        GdiPlus();
        ~GdiPlus() noexcept;

        GdiPlus(const GdiPlus&) = delete;
        GdiPlus(GdiPlus&&) = delete;

        GdiPlus& operator=(const GdiPlus&) = delete;
        GdiPlus& operator=(GdiPlus&&) = delete;

    private:
        Gdiplus::GdiplusStartupInput gdiplusStartupInput_;
        ULONG_PTR gdiplusToken_;
    };

    class ImageStream final
    {
    public:
        ImageStream();
        ImageStream(const Image& image);
        ImageStream(ImageStream&& other) noexcept;
        ImageStream(const ImageStream& ) = delete;
        ~ImageStream() noexcept;

        ImageStream& operator=(ImageStream&& other) noexcept;
        ImageStream& operator=(const ImageStream& other) = delete;

        [[nodiscard]] IStream* get();
        [[nodiscard]] size_t size() const;

        void write(const Image& image);
        void read(Image& image) const;

    private:
        void allocate();

    private:
        IStream* stream_ = nullptr;
    };

    ImageStream& operator<<(ImageStream& stream, const Image& image);
    ImageStream& operator>>(ImageStream& stream, Image& image);

} // Distance::Media::GdiplusHelper
