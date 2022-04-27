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
// File: ServerHandlers.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#pragma once

#include "Server.hpp"
#include "Logger.hpp"


namespace Distance::Network::Handler
{

    /*
    * @brief The AcceptHandler function is routine for handled new connections
    * to the server
    *
    * @param[in] server - shared pointer to the server object, to control it
    *
    * @param[in,out] msg - reference to the message
    *
    * @param[in] ec - constant reference to an error code from the async_accept method
    */
    void AcceptHandler(
        std::shared_ptr<Server> server,
        Message& msg,
        const boost::system::error_code& ec
    );


    /*
    * @brief The IsClientDisconnect function checks if the error is related
    * to the client disconnect or not
    *
    * @param[in] ec - constant reference to an error code
    *
    * @return true, if client was disconnected, otherwise false
    */
    bool IsClientDisconnect(const boost::system::error_code& ec);

} // Distance::Network::Handler