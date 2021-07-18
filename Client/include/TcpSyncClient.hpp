#ifndef _DISTANCE_TCP_CLIENT_HPP_
#define _DISTANCE_TCP_CLIENT_HPP_

#include <cstdint>
#include <string>

#include <boost/asio.hpp>
#include <boost/json.hpp>

#include "ClientConfigurator.hpp"
#include "Communication.hpp"

//#include <iostream>



namespace Distance
{

    //---------------------------------------------------------
    // This class is the interface for working with the server
    //---------------------------------------------------------
    class TcpSyncClient
    {
    public:
        TcpSyncClient(boost::asio::io_context& io);
        TcpSyncClient(
            boost::asio::io_context& io,
            const boost::asio::ip::tcp& protocol,
            uint16_t port
        );
        TcpSyncClient(boost::asio::io_context& io, const TcpClientSettings& settings);

        TcpSyncClient(const TcpSyncClient&) = delete;
        TcpSyncClient& operator=(const TcpSyncClient&) = delete;

        void acceptQuery();

        size_t readQuery(Query* buf);
        size_t sendQuery(Query buf);

        size_t dataSend(void* buf, size_t size);
        size_t dataSend(std::string& buf);
        size_t dataSend(std::unique_ptr<char[]>& buf, size_t size);
        size_t dataSend(std::wstring& buf);
        size_t dataSend(std::unique_ptr<wchar_t[]>& buf, size_t size);

        size_t dataReceive(void* buf, size_t size);
        size_t dataReceive(std::unique_ptr<char[]>& buf, size_t size);

        uint16_t getPort() const;

    private:
        boost::asio::io_context& io_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::ip::tcp::socket socket_;
        uint16_t port_;

    };



} // namespace Distance
#endif // _DISTANCE_TCP_CLIENT_HPP_
