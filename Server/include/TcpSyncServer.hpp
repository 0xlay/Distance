#ifndef _DISTANCE_TCP_ASYNC_SERVER_HPP_
#define _DISTANCE_TCP_ASYNC_SERVER_HPP_

#include <memory>
#include <sstream>
#include <cstdint>

#include <boost/asio.hpp>
#include <boost/json.hpp>

#include "ServerConfigurator.hpp"
#include "Communication.hpp"



namespace Distance
{

    class TcpSyncServer
    {
    public:
        TcpSyncServer(
            boost::asio::io_context& io,
            const std::string& ip,
            uint16_t port = 25565
        );
        TcpSyncServer(boost::asio::io_context& io, const TcpServerSettings& settings);

        TcpSyncServer(const TcpSyncServer&) = delete;
        TcpSyncServer& operator=(const TcpSyncServer&) = delete;

        void connectToClient();

        [[nodiscard]] bool isWorking() const;
        [[nodiscard]] uint16_t getPort() const;
        [[nodiscard]] const char* getIp() const;

        size_t readQuery(Query* buf);
        size_t sendQuery(Query query);

        size_t dataSend(void* buf, size_t size);
        size_t dataSend(std::string& buf);

        size_t dataReceive(void* buf, size_t size);
        size_t dataReceive(std::unique_ptr<char[]>& buf, size_t size);
        size_t dataReceive(std::unique_ptr<wchar_t[]>& buf, size_t size);

    private:
        boost::asio::io_context& io_;
        boost::asio::ip::tcp::resolver resolver_;
        boost::asio::ip::tcp::socket socket_;
        boost::asio::ip::tcp::resolver::results_type endpoints_;

        std::string ip_;
        uint16_t port_;

        bool work_ = false;
    };

} // namespace Distance
#endif // _DISTANCE_TCP_ASYNC_SERVER_HPP_
