#include "pch.hpp"
#include "DefaultTcpServer.hpp"


using namespace boost::asio;


namespace Distance::Network
{

	DefaultTcpServer::DefaultTcpServer(std::unique_ptr<ServerSettings> settings)
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

	std::uint16_t DefaultTcpServer::getPort() const
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

		sockets_[clientNumber].async_send(buffer(msg.getData()), asioHandler);
	}

	void DefaultTcpServer::asyncReceive(Message& outMsg, ReceiveHandler handler)
	{
		assert(sockets_.size() > 0);

		size_t clientNumber = sockets_.size() - 1;
		auto asioHandler = std::bind(
			handler, shared_from_this(), clientNumber, std::ref(outMsg),
			std::placeholders::_1, std::placeholders::_2
		);

		sockets_[clientNumber].async_receive(buffer(outMsg.getData()), asioHandler);
	}

	bool DefaultTcpServer::send(const Message& msg, size_t clientNumber)
	{
		if (clientNumber >= sockets_.size()) return false;
		boost::system::error_code ec;
		sockets_[clientNumber].send(buffer(msg.getData()), 0, ec);
		return !ec;
	}

	bool DefaultTcpServer::send(MessageHeader header, size_t clientNumber)
	{
		if (clientNumber >= sockets_.size()) return false;
		boost::system::error_code ec;
		sockets_[clientNumber].send(buffer(static_cast<void*>(&header), sizeof(header)), 0, ec);
		return !ec;
	}

	bool DefaultTcpServer::sendAll(const Message& msg, size_t clientNumber)
	{
		boost::system::error_code ec;

		if (!send(msg.getMsgHeader(), clientNumber))
			return false;

		return send(msg, clientNumber);
	}

	bool DefaultTcpServer::receive(Message& msg, size_t clientNumber)
	{
		if (clientNumber >= sockets_.size()) return false;
		boost::system::error_code ec;
		sockets_[clientNumber].receive(buffer(msg.getData()), 0, ec);
		return !ec;
	}

	bool DefaultTcpServer::receive(MessageHeader& header, size_t clientNumber)
	{
		if (clientNumber >= sockets_.size()) return false;
		boost::system::error_code ec;
		sockets_[clientNumber].receive(buffer(static_cast<void*>(&header), sizeof(header)), 0, ec);
		return !ec;
	}

	bool DefaultTcpServer::receiveAll(Message& msg, size_t clientNumber)
	{
		boost::system::error_code ec;
		MessageHeader header;

		if (!receive(header, clientNumber))
			return false;

		msg.resize(header.size);
		msg.setMsgType(header.type);

		return receive(msg, clientNumber);
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

} // namespace Distance::Network
