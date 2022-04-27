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
// File: ServerSettings.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#include "pch.hpp"
#include "DefaultTcpServer.hpp"


using namespace boost::asio;


namespace Distance::Network
{

    DefaultTcpServer::DefaultTcpServer(std::shared_ptr<ServerSettings> settings)
        : settings_(std::move(settings))
    {
        switch (settings_->getProtocol())
        {
        case InternetProtocol::IPV4:
        {
            ip::tcp::endpoint ep(AsioTCP::v4(), settings_->getPort());
            acceptor_ = std::make_unique<AsioTCP::acceptor>(io_, std::move(ep));
            break;
        }
        case InternetProtocol::IPV6:
        {
            ip::tcp::endpoint ep(AsioTCP::v6(), settings_->getPort());
            acceptor_ = std::make_unique<AsioTCP::acceptor>(io_, std::move(ep));
            break;
        }
        default:
            assert(false);
        }
    }

    void DefaultTcpServer::setHandler(AcceptHandler handler, Message& msg)
    {
        acceptHandler_ = handler;
        startAccept(msg);
    }

    void DefaultTcpServer::run()
    {
        io_.run();
    }

    void DefaultTcpServer::stop()
    {
        std::lock_guard lock(mutex_);
        boost::system::error_code ec;
        for (auto& sock : sockets_)
            sock.cancel(ec);
        sockets_.clear();
        io_.stop();
    }

    size_t DefaultTcpServer::clientsQuantity() const
    {
        return (sockets_.size() - 1 < 0) ? 0 : sockets_.size() - 1;
    }

    std::uint32_t DefaultTcpServer::getPort() const
    {
        return settings_->getPort();
    }

    InternetProtocol DefaultTcpServer::getProtocol() const
    {
        return settings_->getProtocol();
    }
    
    void DefaultTcpServer::asyncSend(const Message& msg, SendHandler handler)
    {
        assert(sockets_.size() > 0);

        size_t clientNumber = sockets_.size() - 1;
        auto asioHandler = std::bind(
            handler, shared_from_this(), clientNumber, std::cref(msg),
            std::placeholders::_1, std::placeholders::_2
        );

        sockets_[clientNumber].async_send(buffer(msg.getMemory()), asioHandler);
    }

    void DefaultTcpServer::asyncReceive(Message& outMsg, ReceiveHandler handler)
    {
        assert(sockets_.size() > 0);

        size_t clientNumber = sockets_.size() - 1;
        auto asioHandler = std::bind(
            handler, shared_from_this(), clientNumber, std::ref(outMsg),
            std::placeholders::_1, std::placeholders::_2
        );

        sockets_[clientNumber].async_receive(buffer(outMsg.getMemory()), asioHandler);
    }

    bool DefaultTcpServer::send(const Message& msg, size_t clientNumber)
    {
        if (!sendImpl(msg.getMsgHeader(), clientNumber))
            return false;

        return (msg.size() ? sendImpl(msg, clientNumber) : true);
    }

    bool DefaultTcpServer::receive(Message& msg, size_t clientNumber)
    {
        MessageHeader header;

        if (!receiveImpl(header, clientNumber))
            return false;

        msg.setMsgHeader(header);
        msg.resize(header.size);

        return (header.size ? receiveImpl(msg, clientNumber) : true);
    }

    void DefaultTcpServer::startAccept(Message& msg)
    {
        sockets_.emplace_back(AsioTcpSocket(io_));
        acceptor_->async_accept(
            sockets_.back(),
            std::bind(acceptHandler_, shared_from_this(), std::ref(msg), std::placeholders::_1)
        );
    }

    std::deque<Server::AsioTcpSocket>* DefaultTcpServer::getSockets()
    {
        return &sockets_;
    }

    bool DefaultTcpServer::sendImpl(const Message& msg, size_t clientNumber)
    {
        if (clientNumber >= sockets_.size()) return false;
        boost::system::error_code ec;
        sockets_[clientNumber].send(buffer(msg.getMsgInBytes(), msg.size()), 0, ec);
        return !ec;
    }

    bool DefaultTcpServer::sendImpl(MessageHeader header, size_t clientNumber)
    {
        if (clientNumber >= sockets_.size()) return false;
        boost::system::error_code ec;
        sockets_[clientNumber].send(buffer(static_cast<void*>(&header), sizeof(header)), 0, ec);
        return !ec;
    }

    bool DefaultTcpServer::receiveImpl(Message& msg, size_t clientNumber)
    {
        if (clientNumber >= sockets_.size()) return false;
        boost::system::error_code ec;
        sockets_[clientNumber].receive(buffer(msg.getMsgInBytes(), msg.size()), 0, ec);
        return !ec;
    }

    bool DefaultTcpServer::receiveImpl(MessageHeader& header, size_t clientNumber)
    {
        if (clientNumber >= sockets_.size()) return false;
        boost::system::error_code ec;
        sockets_[clientNumber].receive(buffer(static_cast<void*>(&header), sizeof(header)), 0, ec);
        return !ec;
    }

} // namespace Distance::Network
