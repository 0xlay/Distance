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
// File: MediaCommand.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "MediaCommand.hpp"
#include "Media.hpp"

namespace Distance::Command
{
    CreateScreenshot::CreateScreenshot()
        : pipeClient_(Media::g_PipeName, xstar::PipeAccess::InOut, Media::g_PipeBufSize)
    {
        pipeClient_.connect();
    }

    CreateScreenshot& CreateScreenshot::execute()
    {
        pipeClient_.write(static_cast<int>(Media::Command::Screenshot));
        pipeClient_.read(data_);
        return *this;
    }

    std::vector<char> CreateScreenshot::pickUp() noexcept
    {
        return std::move(data_);
    }

} // Distance::Command
