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
// File: DefaultTcpClient.hpp
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
#include "ClientSettings.hpp"


namespace Distance::Network
{

    class DefaultTcpClient : public Client
    {
    public:
        DefaultTcpClient(std::shared_ptr<ClientSettings> settings)
            : settings_(std::move(settings)), socket_(io_)
        { }

        void setHandler(ConnectHandler handler, Message& msg) override;
        void run() override;
        void stop() override;
        void reset() override;

        [[nodiscard]] std::string_view getIp() const override;
        [[nodiscard]] std::uint32_t getPort() const override;

        void asyncSend(const Message& msg, SendHandler handler) override;
        void asyncReceive(Message& outMsg, ReceiveHandler handler) override;

        [[nodiscard]] bool send(const Message& msg) override;
        [[nodiscard]] bool receive(Message& msg) override;

    protected:
        [[nodiscard]] bool sendImpl(MessageHeader header);
        [[nodiscard]] bool sendImpl(const Message& msg);
        [[nodiscard]] bool receiveImpl(MessageHeader& header);
        [[nodiscard]] bool receiveImpl(Message& msg);

    protected:
        std::mutex mutex_;
        std::shared_ptr<ClientSettings> settings_;
        AsioTcpSocket socket_;
    };

} // Distance::Network