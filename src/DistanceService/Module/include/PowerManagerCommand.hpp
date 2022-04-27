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
// File: PowerManagerCommand.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#pragma once

#include "ICommand.hpp"
#include "PowerManagerModule.hpp"

namespace Distance::Command
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // The ShutdownPc command
    //
    ////////////////////////////////////////////////////////////////////////////
 
    class ShutdownPc : public ICommand
    {
    public:
        ShutdownPc();
        ShutdownPc& execute() override;

    private:
        std::shared_ptr<Module::PowerManager> pm_;
    };

    ////////////////////////////////////////////////////////////////////////////
    //
    // The RebootPc command
    //
    ////////////////////////////////////////////////////////////////////////////

    class RebootPc : public ICommand
    {
    public:
        RebootPc();
        RebootPc& execute() override;

    private:
        std::shared_ptr<Module::PowerManager> pm_;
    };

}; // Distance::Command
