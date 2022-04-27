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
// File: ClientHandler.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#pragma once

#include "Client.hpp"
#include "Logger.hpp"


namespace Distance::Network::Handler
{

    /*
    * @brief The Connect function is routine for handled new connections
    * to the client
    *
    * @param[in] client - shared pointer to the client object, to control it
    *
    * @param[in,out] msg - reference to the message
    *
    * @param[in] ec - constant reference to an error code from the async_connect method
    */
    void Connect(
        std::shared_ptr<Client> client,
        Message& msg,
        const boost::system::error_code& ec
    );

    /*
    * @brief The Connect function is routine for handled new connections
    * to the client
    *
    * @param[in] client - shared pointer to the client object, to control it
    *
    * @param[in,out] msg - reference to the message
    *
    * @param[in] ec - constant reference to an error code from the async_receive method
    * 
    * @param[in] size - received number of bytes
    */
    void Receive(
        std::shared_ptr<Client> client,
        Message& msg,
        const boost::system::error_code& ec,
        size_t size
    );

} // Distance::Network::Handler
