#include "ClientNetwork.hpp"
#include "DefaultTcpClient.hpp"


using namespace boost::asio;


namespace Distance::Network
{

	void DefaultTcpClient::setHandler(ConnectHandler handler, Message& msg)
	{
		ip::tcp::endpoint ep(
			ip::address::from_string(settings_->getIp().data()),
			settings_->getPort()
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
		socket_.cancel(ec);
		io_.stop();
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

	std::uint16_t DefaultTcpClient::getPort() const
	{
		return settings_->getPort();
	}

	void DefaultTcpClient::asyncSend(const Message& msg, SendHandler handler)
	{
		socket_.async_send(
			buffer(msg.getData()),
			std::bind(handler, shared_from_this(), std::cref(msg), std::placeholders::_1, std::placeholders::_2)
		);
	}

	void DefaultTcpClient::asyncReceive(Message& outMsg, ReceiveHandler handler)
	{
		socket_.async_receive(
			buffer(outMsg.getData()),
			std::bind(handler, shared_from_this(), std::ref(outMsg), std::placeholders::_1, std::placeholders::_2)
		);
	}

	bool DefaultTcpClient::send(const Message& msg)
	{
		boost::system::error_code ec;
		socket_.send(buffer(msg.getData()), 0, ec);
		return !ec;
	}

	bool DefaultTcpClient::send(MessageHeader header)
	{
		boost::system::error_code ec;
		socket_.send(buffer(static_cast<void*>(&header), sizeof(header)), 0, ec);
		return !ec;
	}

	bool DefaultTcpClient::sendAll(const Message& msg)
	{
		boost::system::error_code ec;

		if (!send(msg.getMsgHeader()))
			return false;
		
		return send(msg);
	}

	bool DefaultTcpClient::receive(Message& msg)
	{
		boost::system::error_code ec;
		socket_.receive(buffer(msg.getData()), 0, ec);
		return !ec;
	}

	bool DefaultTcpClient::receive(MessageHeader& header)
	{
		boost::system::error_code ec;
		socket_.receive(buffer(static_cast<void*>(&header), sizeof(header)), 0, ec);
		return !ec;
	}

	bool DefaultTcpClient::receiveAll(Message& msg)
	{
		boost::system::error_code ec;
		MessageHeader header;

		if (!receive(header))
			return false;

		msg.resize(header.size);
		msg.setMsgType(header.type);

		return receive(msg);
	}
} // namespace Distance::Network
