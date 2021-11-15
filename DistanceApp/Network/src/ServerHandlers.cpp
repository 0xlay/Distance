#include "pch.hpp"
#include "ServerHandlers.hpp"
#include "DefaultTcpServer.hpp"
#include "Json.hpp"


namespace Distance::Network::Handler
{

	void AcceptHandler(
		std::shared_ptr<Server> server,
		Message& msg,
		const boost::system::error_code& ec
	)
	{
		auto tcpServer = std::static_pointer_cast<DefaultTcpServer>(server);
		if (ec)
		{
			LOG_INFO("The server failed to accept a client! Error message: ", ec.message())
			return;
		}

		tcpServer->startAccept(msg);
	}



	bool IsClientDisconnect(const boost::system::error_code& ec)
	{
		return ec == boost::asio::error::bad_descriptor || \
			ec == boost::asio::error::connection_reset || \
			ec == boost::asio::error::connection_aborted || \
			ec == boost::asio::error::eof;
	}

} // Distance::Network::Handler
