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
// File: ServerHandlers.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#include "pch.hpp"
#include "ServerHandlers.hpp"
#include "DefaultTcpServer.hpp"
#include "Json.hpp"

using Distance::Utils::LOG_INFO;

namespace Distance::Network::Handler
{

    void AcceptHandler(
        std::shared_ptr<Server> server,
        Message& msg,
        const boost::system::error_code& ec
    )
    {
        auto tcpServer = std::static_pointer_cast<DefaultTcpServer>(server);
        if (ec)
        {
            LOG_INFO("The server failed to accept a client! Error message: ", ec.message());
            return;
        }

        tcpServer->startAccept(msg);
    }



    bool IsClientDisconnect(const boost::system::error_code& ec)
    {
        return ec == boost::asio::error::bad_descriptor || \
               ec == boost::asio::error::connection_reset || \
               ec == boost::asio::error::connection_aborted || \
               ec == boost::asio::error::eof;
    }

} // Distance::Network::Handler
