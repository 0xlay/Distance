#pragma once

#include "Configurator.hpp"


namespace Distance::Network
{

    enum class InternetProtocol
    {
        IPV4, IPV6
    };

    namespace _internal
    {
        /*
        * @brief The ServerSettingsPack struct keeps the configuration that is needed
        * for the server part
        */
        struct ServerSettingsPack
        {
            InternetProtocol protocol{ InternetProtocol::IPV4 };
            std::uint16_t port{ 2021 };
        };
    }

    /*
    * @brief The ServerSettings class for working with server part settings
    */
    class ServerSettings : public Settings
    {
    public:
        ServerSettings() = default;
        ServerSettings(InternetProtocol protocol, std::uint16_t port)
            : settings_{ protocol, port }
        { }

        void save(std::fstream& file) override;

        void load(std::fstream& file) override;

        void setProtocol(InternetProtocol protocol);
        void setPort(std::uint16_t port);
        [[nodiscard]] InternetProtocol getProtocol() const;
        [[nodiscard]] std::uint16_t getPort() const;

    private:
        _internal::ServerSettingsPack settings_;
    };

} // Distance::Network