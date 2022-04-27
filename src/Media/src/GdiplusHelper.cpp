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
// File: GdiplusHelper.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Media.exe
//
//------------------------------------------------------------------------------

#include "GdiplusHelper.hpp"
#include <memory>


namespace Distance::Media::GdiplusHelper
{

    int GetEncoderClsid(const wchar_t* format, CLSID* clsid)
    {
        UINT num = 0; // number of image encoders
        UINT size = 0; // size of the image encoder array in bytes

        Gdiplus::GetImageEncodersSize(&num, &size);
        if (!size)
        {
            return -1;
        }

        auto imgCodecInfo = std::unique_ptr<Gdiplus::ImageCodecInfo>(new Gdiplus::ImageCodecInfo[size]);

        Gdiplus::GetImageEncoders(num, size, imgCodecInfo.get());

        for (UINT i = 0; i < num; ++i)
        {
            if (wcscmp((imgCodecInfo.get() + i)->MimeType, format) == 0)
            {
                *clsid = (imgCodecInfo.get() + i)->Clsid;
                return i; // Success
            }
        }

        return -1;
    }



    ////////////////////////////////////////////////////////////////////////////
    //
    // The GdiPlus implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    GdiPlus::GdiPlus()
        : gdiplusStartupInput_{}, gdiplusToken_{}
    {
        if (Gdiplus::GdiplusStartup(&gdiplusToken_, &gdiplusStartupInput_, nullptr) != Gdiplus::Ok)
        {
            throw std::runtime_error("Failed to initialize GDI+!");
        }
    }

    GdiPlus::~GdiPlus() noexcept
    {
        Gdiplus::GdiplusShutdown(gdiplusToken_);
    }



    ////////////////////////////////////////////////////////////////////////////
    //
    // The ImageStream implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    ImageStream::ImageStream()
    {
        allocate();
    }

    ImageStream::ImageStream(const Image& image)
    {
        allocate();
        write(image);
    }

    ImageStream::ImageStream(ImageStream&& other) noexcept
    {
        stream_ = other.stream_;
        other.stream_ = nullptr;
    }

    ImageStream::~ImageStream() noexcept
    {
        if (stream_)
        {
            stream_->Release();
        }
    }

    ImageStream& ImageStream::operator=(ImageStream&& other) noexcept
    {
        stream_->Release();
        stream_ = other.stream_;
        other.stream_ = nullptr;
        return  *this;
    }

    IStream* ImageStream::get()
    {
        return stream_;
    }

    size_t ImageStream::size() const
    {
        STATSTG statstg{};
        stream_->Stat(&statstg, STATFLAG_NONAME);
        return statstg.cbSize.QuadPart;
    }

    void ImageStream::write(const Image& image)
    {
        if (stream_->Seek(LARGE_INTEGER{}, STREAM_SEEK_END, nullptr) != S_OK)
        {
            throw std::ios_base::failure("Failed to move pointer in the stream!");
        }

        ULONG written;
        auto status = stream_->Write(image.get(), static_cast<ULONG>(image.size()), &written);
        if ( status != S_OK || image.size() != written)
        {
            throw std::ios_base::failure("Failed to write image to stream!");
        }
    }

    void ImageStream::read(Image& image) const
    {
        Image img(size());
        if (stream_->Seek(LARGE_INTEGER{}, STREAM_SEEK_SET, nullptr) != S_OK)
        {
            throw std::ios_base::failure("Failed to move pointer in the stream!");
        }

        ULONG read;
        auto status = stream_->Read(image.get(), static_cast<ULONG>(image.size()), &read);
        if (status != S_OK || image.size() != read)
        {
            throw std::ios_base::failure("Failed to read image from stream!");
        }
    }

    void ImageStream::allocate()
    {
        if (CreateStreamOnHGlobal(nullptr, true, &stream_) != S_OK)
        {
            throw std::bad_alloc();
        }
    }

    ImageStream& operator<<(ImageStream& stream, const Image& image)
    {
        stream.write(image);
        return stream;
    }

    ImageStream& operator>>(ImageStream& stream, Image& image)
    {
        stream.read(image);
        return stream;
    }

} // Distance::Media::GdiplusHelper
