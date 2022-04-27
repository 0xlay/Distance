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
// File: ProcessManagerCommand.hpp
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
#include "ProcessManagerModule.hpp"

namespace Distance::Command
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // The Terminate command
    //
    ////////////////////////////////////////////////////////////////////////////

    class Terminate : public ICommand
    {
    public:
        Terminate(DWORD pid);
        Terminate& execute() override;

    private:
        std::shared_ptr<Module::ProcessManager> pm_;
        DWORD pid_;
    };

    ////////////////////////////////////////////////////////////////////////////
    //
    // The ProcessList command
    //
    ////////////////////////////////////////////////////////////////////////////

    class ProcessList : public ICommand
    {
    public:
        ProcessList();

        ProcessList& execute() override;

        [[nodiscard]]
        Module::ProcessList pickUp() noexcept;

    private:
        std::shared_ptr<Module::ProcessManager> pm_;
        Module::ProcessList list_;
    };

} // Distance::Command
