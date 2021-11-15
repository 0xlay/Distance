#pragma once

#include "Configurator.hpp"


namespace Distance::Network
{

    namespace _internal
    {
        /*
        * @brief The "ClientSettingPack" keeps the configuration that is needed
        * for the client part
        */
        struct ClientSettingPack
        {
            std::string ip{ "127.0.0.1" };
            std::uint16_t port{ 2021 };
        };
    }

    /*
    * @brief The "ClientSettings" is the interface for working with client part
    * settings
    */
    class ClientSetting : public Settings
    {
    public:
        ClientSetting() = default;
        ClientSetting(std::string_view ip, std::uint16_t port);
        void load(std::fstream& file) override;
        void save(std::fstream& file) override;

        void setIp(std::string_view ip);
        void setPort(std::uint16_t port);
        [[nodiscard]] std::string_view getIp() const;
        [[nodiscard]] std::uint16_t getPort() const;

    private:
        _internal::ClientSettingPack settings_;
    };

} // Distance::Network