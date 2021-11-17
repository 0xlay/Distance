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
			void (*)(
				std::shared_ptr<Client>,         /* access to client */
				Message&,                        /* message */
				const boost::system::error_code& /* error code */
				);

		using SendHandler =
			void (*)(
				std::shared_ptr<Client>,          /* access to client */
				const Message&,                   /* message */
				const boost::system::error_code&, /* error code */
				size_t                            /* bytes sent */
				);

		using ReceiveHandler =
			void (*)(
				std::shared_ptr<Client>,          /* access to client */
				Message&,                         /* message */
				const boost::system::error_code&, /* error code */
				size_t                            /* bytes received */
				);

		using AsioTcpSocket = boost::asio::ip::tcp::socket;
		using AsioUdpSocket = boost::asio::ip::udp::socket;

	public:
		virtual void setHandler(ConnectHandler handler, Message& msg) = 0;
		virtual void run() = 0;
		virtual void stop() = 0;
		virtual void reset() = 0;

		virtual std::string_view getIp() const = 0;
		virtual std::uint16_t getPort() const = 0;

		virtual void asyncSend(const Message& msg, SendHandler handler) = 0;
		virtual void asyncReceive(Message& outMsg, ReceiveHandler handler) = 0;

		virtual bool send(const Message& msg) = 0;
		virtual bool send(MessageHeader header) = 0;
		virtual bool sendAll(const Message& msg) = 0;

		virtual bool receive(Message& msg) = 0;
		virtual bool receive(MessageHeader& header) = 0;
		virtual bool receiveAll(Message& msg) = 0;

		virtual ~Client() = default;

	protected:
		boost::asio::io_context io_{};
	};

} // namespace Distance::Network
