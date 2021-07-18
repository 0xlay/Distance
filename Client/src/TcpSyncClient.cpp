#include "TcpSyncClient.hpp"

using namespace boost::asio;
using namespace boost::json;
using namespace boost::asio::ip;

namespace Distance
{

    TcpSyncClient::TcpSyncClient(io_context& io)
        : io_(io), port_(25565),
          acceptor_(io_, tcp::endpoint(tcp::v4(), 25565)), socket_(io_)
    { }



    TcpSyncClient::TcpSyncClient(io_context& io, const tcp& protocol, uint16_t port)
        : io_(io), port_(port),
          acceptor_(io_, tcp::endpoint(protocol, port_)), socket_(io)
    { }



    TcpSyncClient::TcpSyncClient(
        boost::asio::io_context& io, const TcpClientSettings& settings
    )
        : io_(io), port_(settings.port), 
          acceptor_(
              io_, 
              tcp::endpoint(settings.protocol == "IPV4" ? tcp::v4() : tcp::v6(), port_)
          ),
          socket_(io_)
    { }



    void TcpSyncClient::acceptQuery()
    {
        acceptor_.accept(socket_);
    }



    size_t TcpSyncClient::readQuery(Query* buf)
    {
        return socket_.receive(buffer(buf, sizeof(int)));
    }



    size_t TcpSyncClient::sendQuery(Query buf)
    {
        return socket_.send(buffer(&buf, sizeof(int)));
    }



    uint16_t TcpSyncClient::getPort() const
    {
        return port_;
    }


    size_t TcpSyncClient::dataSend(void* buf, size_t size)
    {
        return socket_.send(buffer(buf, size));
    }



    size_t TcpSyncClient::dataSend(std::string& buf)
    {
        return dataSend(const_cast<char*>(buf.c_str()), buf.length());
    }



    size_t TcpSyncClient::dataSend(std::unique_ptr<char[]>& buf, size_t size)
    {
        return socket_.send(buffer(buf.get(), size));
    }



    size_t TcpSyncClient::dataSend(std::wstring& buf)
    {
        return dataSend(const_cast<wchar_t*>(buf.c_str()), buf.length());
    }



    size_t TcpSyncClient::dataSend(std::unique_ptr<wchar_t[]>& buf, size_t size)
    {
        return socket_.send(buffer(buf.get(), size * sizeof(wchar_t)));
    }



    size_t TcpSyncClient::dataReceive(void* buf, size_t size)
    {
        return socket_.receive(buffer(buf, size));
    }



    size_t TcpSyncClient::dataReceive(std::unique_ptr<char[]>& buf, size_t size)
    {
        return socket_.receive(buffer(buf.get(), size));
    }

} // namespace Distance
