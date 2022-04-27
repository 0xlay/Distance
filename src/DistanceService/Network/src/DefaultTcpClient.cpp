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

#include "ClientNetwork.hpp"
#include "DefaultTcpClient.hpp"


using namespace boost::asio;


namespace Distance::Network
{

    void DefaultTcpClient::setHandler(ConnectHandler handler, Message& msg)
    {
        ip::tcp::endpoint ep(
            ip::address::from_string(settings_->getIp().data()),
            static_cast<ip::port_type>(settings_->getPort())
        );

        socket_.async_connect(ep, std::bind(handler, shared_from_this(), std::ref(msg), std::placeholders::_1));
    }

    void DefaultTcpClient::run()
    {
        io_.run();
    }

    void DefaultTcpClient::stop()
    {
        std::lock_guard lock_(mutex_);
        boost::system::error_code ec;
        io_.stop();
        while(!io_.stopped())
        { }
        socket_.cancel(ec);
        socket_.close(ec);
    }

    void DefaultTcpClient::reset()
    {
        io_.reset();
        socket_ = AsioTcpSocket(io_);
    }

    std::string_view DefaultTcpClient::getIp() const
    {
        return settings_->getIp();
    }

    std::uint32_t DefaultTcpClient::getPort() const
    {
        return settings_->getPort();
    }

    void DefaultTcpClient::asyncSend(const Message& msg, SendHandler handler)
    {
        socket_.async_send(
            buffer(msg.getMemory()),
            std::bind(handler, shared_from_this(), std::cref(msg), std::placeholders::_1, std::placeholders::_2)
        );
    }

    void DefaultTcpClient::asyncReceive(Message& outMsg, ReceiveHandler handler)
    {
        socket_.async_receive(
            buffer(outMsg.getMemory()),
            std::bind(handler, shared_from_this(), std::ref(outMsg), std::placeholders::_1, std::placeholders::_2)
        );
    }

    bool DefaultTcpClient::send(const Message& msg)
    {
        if (!sendImpl(msg.getMsgHeader()))
            return false;
        
        return (msg.size() ? sendImpl(msg) : true);
    }

    bool DefaultTcpClient::receive(Message& msg)
    {
#if 0
        MessageHeader header;

        if (!receiveImpl(header))
            return false;

        msg.setMsgHeader(header);
        msg.resize(header.size);

        return (header.size ? receiveImpl(msg) : true);
#endif

        msg.resize(msg.size());
        return receiveImpl(msg);
    }

    bool DefaultTcpClient::sendImpl(const Message& msg)
    {
        boost::system::error_code ec;
        socket_.send(buffer(msg.getMsgInBytes(), msg.size()), 0, ec);
        return !ec;
    }

    bool DefaultTcpClient::sendImpl(MessageHeader header)
    {
        boost::system::error_code ec;
        socket_.send(buffer(static_cast<void*>(&header), sizeof(header)), 0, ec);
        return !ec;
    }

    bool DefaultTcpClient::receiveImpl(Message& msg)
    {
        boost::system::error_code ec;
        socket_.receive(buffer(msg.getMsgInBytes(), msg.size()), 0, ec);
        return !ec;
    }

    bool DefaultTcpClient::receiveImpl(MessageHeader& header)
    {
        boost::system::error_code ec;
        socket_.receive(buffer(static_cast<void*>(&header), sizeof(header)), 0, ec);
        return !ec;
    }

} // namespace Distance::Network
