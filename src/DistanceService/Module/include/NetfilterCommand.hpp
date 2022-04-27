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
// File: NetfilterCommand.hpp
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
#include "NetfilterModule.hpp"

namespace Distance::Command
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // The BlockTcpPorts command
    //
    ////////////////////////////////////////////////////////////////////////////

    class BlockTcpPorts : public ICommand
    {
    public:
        BlockTcpPorts(const std::vector<UINT16>& ports);
        BlockTcpPorts& execute() override;

    private:
        std::vector<UINT16> ports_;
        std::shared_ptr<Module::Netfilter> netflt_;
    };

    ////////////////////////////////////////////////////////////////////////////
    //
    // The UnblockTcpPorts command
    //
    ////////////////////////////////////////////////////////////////////////////

    class UnblockTcpPorts : public ICommand
    {
    public:
        UnblockTcpPorts();
        UnblockTcpPorts& execute() override;

    private:
        std::shared_ptr<Module::Netfilter> netflt_;
    };

} // Distance::Command
