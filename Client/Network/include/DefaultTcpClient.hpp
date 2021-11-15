#pragma once

#include "Client.hpp"
#include "ClientSetting.hpp"


namespace Distance::Network
{

	class DefaultTcpClient : public Client
	{
	public:
		DefaultTcpClient(std::unique_ptr<ClientSetting> settings)
			: settings_(std::move(settings)), socket_(io_)
		{ }

		void setHandler(ConnectHandler handler, Message& msg) override;
		void run() override;
		void stop() override;
		void reset() override;

		[[nodiscard]] std::string_view getIp() const override;
		[[nodiscard]] std::uint16_t getPort() const override;

		void asyncSend(const Message& msg, SendHandler handler) override;
		void asyncReceive(Message& outMsg, ReceiveHandler handler) override;

		[[nodiscard]] bool send(const Message& msg) override;
		[[nodiscard]] bool send(MessageHeader header) override;
		[[nodiscard]] bool sendAll(const Message& msg) override;

		[[nodiscard]] bool receive(Message& msg) override;
		[[nodiscard]] bool receive(MessageHeader& header) override;
		[[nodiscard]] bool receiveAll(Message& msg) override;

	protected:
		std::mutex mutex_;
		std::unique_ptr<ClientSetting> settings_;
		AsioTcpSocket socket_;
	};

}