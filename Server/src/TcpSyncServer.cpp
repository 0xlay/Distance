#include "TcpSyncServer.hpp"

using namespace boost::asio;
using namespace boost::json;
using namespace boost::asio::ip;
using namespace boost::asio::detail::socket_option;



namespace Distance
{

    TcpSyncServer::TcpSyncServer(
        io_context& io, 
        const std::string& ip, 
        uint16_t port
    )
        : io_(io), ip_(ip), port_(port), socket_(io_), resolver_(io_)
    {
        tcp::endpoint point(address::from_string(ip_), port_);
        endpoints_ = resolver_.resolve(point);
    }



    TcpSyncServer::TcpSyncServer(boost::asio::io_context& io, const TcpServerSettings& settings)
        : io_(io), ip_(settings.ip), port_(settings.port), socket_(io_), resolver_(io_)
    {
        const tcp::endpoint point(address::from_string(ip_), port_);
        endpoints_ = resolver_.resolve(point);
    }



    void TcpSyncServer::connectToClient()
    {
        connect(socket_, endpoints_);
        work_ = true;
    }



    bool TcpSyncServer::isWorking() const
    {
        return work_;
    }



    uint16_t TcpSyncServer::getPort() const
    {
        return port_;
    }



    const char* TcpSyncServer::getIp() const
    {
        return ip_.c_str();
    }



    size_t TcpSyncServer::readQuery(Query* buf)
    {
        return socket_.receive(buffer(buf, sizeof(int)));
    }



    size_t TcpSyncServer::sendQuery(Query buf)
    {
        return socket_.send(buffer(&buf, sizeof(int)));
    }



    size_t TcpSyncServer::dataSend(void* buf, size_t size)
    {
        return socket_.send(buffer(buf, size));
    }



    size_t TcpSyncServer::dataSend(std::string& buf)
    {
        return socket_.send(buffer(buf));
    }



    size_t TcpSyncServer::dataReceive(void* buf, size_t size)
    {
        return socket_.receive(buffer(buf, size));
    }



    size_t TcpSyncServer::dataReceive(std::unique_ptr<char[]>& buf, size_t size)
    {
        return socket_.receive(buffer(buf.get(), size));
    }



    size_t TcpSyncServer::dataReceive(std::unique_ptr<wchar_t[]>& buf, size_t size)
    {
        return socket_.receive(buffer(buf.get(), size * sizeof(wchar_t)));
    }

} // namespace Distance
