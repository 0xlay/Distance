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
// File: Server.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Network.lib
//
//------------------------------------------------------------------------------

#pragma once

#include "Message.hpp"


namespace Distance::Network
{
    enum class InternetProtocol;

    /*
    * The Server class it is an interface, which must implement any server
    */
    class Server : public std::enable_shared_from_this<Server>
    {
    protected:
        using AcceptHandler =
        std::function<void(
            std::shared_ptr<Server>,         /* access to server */
            Message&,                        /* message */
            const boost::system::error_code& /* error code */
            )>;

        using SendHandler =
        std::function<void(
            std::shared_ptr<Server>,          /* access to server */
            size_t,                           /* client number */
            const Message&,                   /* message */
            const boost::system::error_code&, /* error code */
            size_t                            /* bytes received */
            )>;

        using ReceiveHandler =
        std::function<void(
            std::shared_ptr<Server>,          /* access to server */
            size_t,                           /* client number */
            Message&,                         /* message */
            const boost::system::error_code&, /* error code */
            size_t                            /* bytes sent */
            )>;

        using AsioTcpSocket = boost::asio::ip::tcp::socket;
        using AsioUdpSocket = boost::asio::ip::udp::socket;

    public:
        virtual void setHandler(AcceptHandler handler, Message& msg) = 0;
        virtual void run() = 0;
        virtual void stop() = 0;

        virtual size_t clientsQuantity() const = 0;
        virtual std::uint32_t getPort() const = 0;
        virtual InternetProtocol getProtocol() const = 0;

        virtual void asyncSend(const Message& msg, SendHandler handler) = 0;
        virtual void asyncReceive(Message& outMsg, ReceiveHandler handler) = 0;

        virtual bool send(const Message& msg, size_t clientNumber) = 0;
        virtual bool receive(Message& msg, size_t clientNumber) = 0;

        virtual ~Server() = default;

    protected:
        boost::asio::io_context io_{};
    };

} // namespace Distance::Network