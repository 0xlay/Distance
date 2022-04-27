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
// File: EntryPoint.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Media.exe
//
//------------------------------------------------------------------------------

#include "Screenshot.hpp"
#include "named_pipe.hpp"
#include "Media.hpp"
#include <sstream>


using namespace Distance;


int
WINAPI
wWinMain(
    [[maybe_unused]] HINSTANCE hInstance,
    [[maybe_unused]] HINSTANCE hPrevInstance,
    [[maybe_unused]] LPWSTR pCmdLine,
    [[maybe_unused]] int nCmdShow
)
{
    xstar::NamedPipeServer pipe(
        Media::g_PipeName, xstar::PipeDirection::InOut, Media::g_PipeBufSize
    );
    try
    {
        while (true)
        {
            pipe.wait();
            int command = 0;
            pipe.read(command);
            switch (static_cast<Media::Command>(command))
            {
                case Media::Command::Screenshot:
                {
                    Media::Image png = Media::ConvertToPng(Media::CreateScreenshot());
                    pipe.write(png.data());
                    break;
                }
                case Media::Command::Close:
                {
                    return 0;
                }
                default:
                {
                    pipe.write("[Media.exe]: Unknown command!");
                    break;
                }
            }
            pipe.stopWait();
        }
    }
    catch (std::exception& ex)
    {
        pipe.write((std::ostringstream() << "[Media.exe]: " << ex.what()).str());
    }
    return 0;
}
