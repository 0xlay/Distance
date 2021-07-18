#ifndef _DISTANCE_SERVER_CONFIGURATOR_HPP_
#define _DISTANCE_SERVER_CONFIGURATOR_HPP_

#include <boost/json.hpp>
#include <fstream>
#include <string>
#include <exception>
#include <stdexcept>

namespace Distance
{

    struct TcpServerSettings
    {
        std::string ip;
        std::uint16_t port;
    };

    class ServerConfigurator
    {
    public:
        ServerConfigurator(const char* path);
        void loadSettings(std::vector<TcpServerSettings>& settings) const;
        void saveSettings(const std::vector<TcpServerSettings>& settings) const;

    private:
        std::string path_;
    };

} // Distance
#endif // _DISTANCE_SERVER_CONFIGURATOR_HPP_