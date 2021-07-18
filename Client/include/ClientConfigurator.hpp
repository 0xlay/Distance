#ifndef _DISTANCE_CLIENT_CONFIGURATOR_HPP_
#define _DISTANCE_CLIENT_CONFIGURATOR_HPP_


#include <boost/json.hpp>
#include <boost/asio.hpp>

#include <fstream>
#include <exception>
#include <stdexcept>


namespace Distance
{
    struct TcpClientSettings
    {
        std::string protocol;
        std::uint16_t port = 0;
    };


    class ClientConfigurator
    {
    public:
        ClientConfigurator(const char* path);
        void loadSettings(TcpClientSettings& settings) const;
        void saveSettings(const TcpClientSettings& settings) const;

    private:
        std::string path_;
    };

} // Distance
#endif // _DISTANCE_CLIENT_CONFIGURATOR_HPP_