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
// File: Screenshot.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Media.exe
//
//------------------------------------------------------------------------------

#include "Screenshot.hpp"
#include "GdiWrapper.hpp"
#include "GdiplusHelper.hpp"
#include <stdexcept>
#include <sstream>


namespace Distance::Media
{

    Image CreateScreenshot()
    {
        // 
        // Set DPI
        //
        if (!SetProcessDPIAware())
        {
            throw std::runtime_error(
                (std::ostringstream() 
                << "Set DPI was failed!" 
                << ::GetLastError()).str()
            );
        }

        //
        // Get width and height of display
        //
        int width = GetSystemMetrics(SM_CXSCREEN);
        if(!width)
        {
            throw std::runtime_error("Get system metrics was failed!");
        }
        int height = GetSystemMetrics(SM_CYSCREEN);
        if (!height)
        {
            throw std::runtime_error("Get system metrics was failed!");
        }

        //
        // retrieves a handle to a desktop
        //
        GdiWrapper::HDC desktop = GetDC(GetDesktopWindow());
        desktop.releaseOnExit(GetDesktopWindow());
        if (!desktop.dc())
        {
            throw std::runtime_error(
                (std::ostringstream() 
                << "Retrieve a handle to a desktop was failed! Error code: "
                << ::GetLastError()).str()
            );
        }

        GdiWrapper::HDC captureDesktop = CreateCompatibleDC(desktop.dc());
        if (!captureDesktop.dc())
        {
            throw std::runtime_error(
                (std::ostringstream()
                << "Create a memory device context was failed! Error code: "
                << ::GetLastError()).str()
            );
        }

        GdiWrapper::HBITMAP captureBitmap = 
            CreateCompatibleBitmap(desktop.dc(), width, height);
        if (!captureBitmap.bitmap())
        {
            throw std::runtime_error(
                (std::ostringstream()
                << "Create a bitmap was failed! Error code: "
                << ::GetLastError()).str()
            );
        }

        if(!SelectObject(captureDesktop.dc(), captureBitmap.bitmap()))
        {
            throw std::runtime_error(
                (std::ostringstream()
                << "Select an object into the specified device context was failed! Error code: "
                << ::GetLastError()).str()
            );
        }

        //
        // performs a bit-block transfer (copy from desktop to captureDesktop)
        //
        if (!BitBlt(
            captureDesktop.dc(), 
            0, 0, 
            width, height,
            desktop.dc(), 
            0, 0, 
            SRCCOPY | CAPTUREBLT
        ))
        {
            throw std::runtime_error(
                (std::ostringstream()
                << "Perform a bit-block transfer of the color data was failed! Error code: "
                << ::GetLastError()).str()
            );
        }

        return CreateBmp(captureBitmap.bitmap());
    }

    Image CreateBmp(HBITMAP bitmap)
    {
        GdiWrapper::HDC hdc = GetDC(nullptr);
        hdc.releaseOnExit(nullptr);
        if (!hdc.dc())
        {
            throw std::runtime_error(
                (std::ostringstream()
                << "Retrieve the DC for the entire screen! Error code: "
                << ::GetLastError()).str()
            );
        }

        BITMAPINFO bmpInfo{};
        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

        if(!GetDIBits(
            hdc.dc(),
            bitmap, 
            0, 
            0, 
            nullptr, 
            &bmpInfo, 
            DIB_RGB_COLORS
        ))
        {
            throw std::runtime_error(
                (std::ostringstream()
                << "Retrieve the bits of the specified compatible bitmap"
                   "and copy them into a buffer was failed! Error code: "
                << ::GetLastError()).str()
            );
        }

        if (bmpInfo.bmiHeader.biSizeImage <= 0)
        {
            bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * abs(bmpInfo.bmiHeader.biHeight) * (bmpInfo.bmiHeader.biBitCount + 7) / 8;
        }

        bmpInfo.bmiHeader.biCompression = BI_RGB;

        BITMAPFILEHEADER bmpFileHeader{};
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfo.bmiHeader.biSizeImage;
        bmpFileHeader.bfType = 'MB';
        bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        Image bmpImg(bmpInfo.bmiHeader.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

        //
        // Copy bitmap file header
        //
        memcpy(bmpImg.get(), &bmpFileHeader, sizeof(BITMAPFILEHEADER));

        //
        // Copy bitmap info header
        //
        memcpy(bmpImg.get() + sizeof(BITMAPFILEHEADER), &bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER));

        //
        // Copy bits
        //
        if (!GetDIBits(
            hdc.dc(),
            bitmap, 0,
            bmpInfo.bmiHeader.biHeight,
            bmpImg.get() + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
            &bmpInfo,
            DIB_RGB_COLORS
        ))
        {
            throw std::runtime_error(
                (std::ostringstream()
                << "Retrieve the bits of the specified compatible bitmap"
                    "and copy them into a buffer was failed! Error code: "
                << ::GetLastError()).str()
            );
        }

        return bmpImg;
    }


    Image ConvertToPng(const Image& bmp)
    {
        //
        // Initialize GDI+.
        //
        GdiplusHelper::GdiPlus gdiPlus;

        CLSID encoderClsid{};
        if (GdiplusHelper::GetEncoderClsid(L"image/png", &encoderClsid) == -1)
        {
            throw std::runtime_error("Failed to get encoder CLSID for PNG!");
        }

        GdiplusHelper::ImageStream bmpStream(bmp);
        GdiplusHelper::ImageStream pngStream;

        auto bmpImg = std::make_unique<Gdiplus::Image>(bmpStream.get());

        Gdiplus::Status status = bmpImg->Save(pngStream.get(), &encoderClsid, nullptr);
        if (status != Gdiplus::Ok)
        {
            throw std::runtime_error(
                (std::ostringstream()
                    << "Retrieve the bits of the specified compatible bitmap"
                    "and copy them into a buffer was failed! Error code: "
                    << status).str()
            );
        }

        Image pngImg(pngStream.size());
        pngStream >> pngImg;
        return pngImg;
    }

} // Distance::Media
