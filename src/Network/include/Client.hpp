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
// File: Client.hpp
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

    /*
    * @brief The Client class it is an interface, which must implement any client
    */
    class Client : public  std::enable_shared_from_this<Client>
    {
    protected:
        using ConnectHandler =
        std::function<void(
            std::shared_ptr<Client>,              /* access to client */
            Message&,                             /* message */
            const boost::system::error_code&      /* error code */
            )>;

        using SendHandler =
        std::function<void(
                std::shared_ptr<Client>,          /* access to client */
                const Message&,                   /* message */
                const boost::system::error_code&, /* error code */
                size_t                            /* bytes sent */
                )>;

        using ReceiveHandler =
        std::function<void(
                std::shared_ptr<Client>,          /* access to client */
                Message&,                         /* message */
                const boost::system::error_code&, /* error code */
                size_t                            /* bytes received */
                )>;

        using AsioTcpSocket = boost::asio::ip::tcp::socket;
        using AsioUdpSocket = boost::asio::ip::udp::socket;

    public:
        virtual void setHandler(ConnectHandler handler, Message& msg) = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
        virtual void reset() = 0;

        virtual std::string_view getIp() const = 0;
        virtual std::uint32_t getPort() const = 0;

        virtual void asyncSend(const Message& msg, SendHandler handler) = 0;
        virtual void asyncReceive(Message& outMsg, ReceiveHandler handler) = 0;
        virtual bool send(const Message& msg) = 0;
        virtual bool receive(Message& msg) = 0;

        virtual ~Client() = default;

    protected:
        boost::asio::io_context io_{};
    };

} // namespace Distance::Network
