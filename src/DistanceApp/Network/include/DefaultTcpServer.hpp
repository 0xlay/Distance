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
// File: DefaultTcpServer.hpp
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
#include "ServerSettings.hpp"


namespace Distance::Network
{

    class DefaultTcpServer : public Server
    {
        using AsioTCP = boost::asio::ip::tcp;
    public:
        DefaultTcpServer(std::shared_ptr<ServerSettings> settings);

        void setHandler(AcceptHandler handler, Message& msg) override;
        void run() override;
        void stop() override;

        [[nodiscard]] size_t clientsQuantity() const override;
        [[nodiscard]] std::uint32_t getPort() const override;
        [[nodiscard]] InternetProtocol getProtocol() const override;

        void asyncSend(const Message& msg, SendHandler handler) override;
        void asyncReceive(Message& outMsg, ReceiveHandler handler) override;

        [[nodiscard]] bool send(const Message& msg, size_t clientNumber) override;
        [[nodiscard]] bool receive(Message& msg, size_t clientNumber) override;

        void startAccept(Message& msg);
        [[nodiscard]] std::deque<AsioTcpSocket>* getSockets();

    protected:        
        [[nodiscard]] bool sendImpl(MessageHeader header, size_t clientNumber);
        [[nodiscard]] bool sendImpl(const Message& msg, size_t clientNumber);
        [[nodiscard]] bool receiveImpl(MessageHeader& header, size_t clientNumber);
        [[nodiscard]] bool receiveImpl(Message& msg, size_t clientNumber);

    protected:
        std::shared_ptr<ServerSettings> settings_;
        std::deque<AsioTcpSocket> sockets_;
        std::unique_ptr<AsioTCP::acceptor> acceptor_;
        AcceptHandler acceptHandler_{};
        std::mutex mutex_;
    };

} // Distance::Network