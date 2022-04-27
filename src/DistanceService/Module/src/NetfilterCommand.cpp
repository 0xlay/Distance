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
// File: NetfilterCommand.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "NetfilterCommand.hpp"
#include "ModulesManager.hpp"

namespace Distance::Command
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // The BlockTcpPorts command implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    BlockTcpPorts::BlockTcpPorts(const std::vector<UINT16>& ports)
        : ports_(ports)
    {
        netflt_ = std::static_pointer_cast<Module::Netfilter>(
            ModulesManager::invoke().getAccess(ModuleType::Netfilter)
        );
    }

    BlockTcpPorts& BlockTcpPorts::execute()
    {
        netflt_->enableBlockTcpPorts(ports_);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // The UnblockTcpPorts command implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    UnblockTcpPorts::UnblockTcpPorts()
    {
        netflt_ = std::static_pointer_cast<Module::Netfilter>(
            ModulesManager::invoke().getAccess(ModuleType::Netfilter)
        );
    }

    UnblockTcpPorts& UnblockTcpPorts::execute()
    {
        netflt_->disableBlockTcpPorts();
        return *this;
    }

} // Distance::Command

