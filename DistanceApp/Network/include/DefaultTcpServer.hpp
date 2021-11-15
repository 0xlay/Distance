#pragma once

#include "Server.hpp"
#include "ServerSettings.hpp"


namespace Distance::Network
{

	class DefaultTcpServer : public Server
	{
		using AsioTCP = boost::asio::ip::tcp;
	public:
		DefaultTcpServer(std::unique_ptr<ServerSettings> settings);

		void setHandler(AcceptHandler handler, Message& msg) override;
		void run() override;
		void stop() override;

		[[nodiscard]] size_t clientsQuantity() const override;
		[[nodiscard]] std::uint16_t getPort() const override;
		[[nodiscard]] InternetProtocol getProtocol() const override;

		void asyncSend(const Message& msg, SendHandler handler) override;
		void asyncReceive(Message& outMsg, ReceiveHandler handler) override;

		[[nodiscard]] bool send(const Message& msg, size_t clientNumber) override;
		[[nodiscard]] bool send(MessageHeader header, size_t clientNumber) override;
		[[nodiscard]] bool sendAll(const Message& msg, size_t clientNumber) override;

		[[nodiscard]] bool receive(Message& msg, size_t clientNumber) override;
		[[nodiscard]] bool receive(MessageHeader& header, size_t clientNumber) override;
		[[nodiscard]] bool receiveAll(Message& msg, size_t clientNumber) override;

		void startAccept(Message& msg);
		[[nodiscard]] std::deque<AsioTcpSocket>* getSockets();

	protected:
		std::unique_ptr<ServerSettings> settings_;
		std::deque<AsioTcpSocket> sockets_;
		std::unique_ptr<AsioTCP::acceptor> acceptor_;
		AcceptHandler acceptHandler_{};
		std::mutex mutex_;
	};

} // Distance::Network