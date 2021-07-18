#include "ClientConfigurator.hpp"

using namespace boost::json;
using namespace boost::asio::ip;

namespace Distance
{
    ClientConfigurator::ClientConfigurator(const char* path)
        : path_(path)
    { }



    void ClientConfigurator::loadSettings(TcpClientSettings& settings) const
    {
        std::ifstream file(path_);
        if (!file.is_open())
            throw std::runtime_error("Ifstream: failed opened file...");

        std::string json_string;
        file >> json_string;

        value js = parse(json_string);
        settings = value_to<TcpClientSettings>(js);

        file.close();
    }



    void ClientConfigurator::saveSettings(const TcpClientSettings& settings) const
    {
        std::ofstream file(path_);
        if (!file.is_open())
            throw std::runtime_error("Ofstream: failed opened file...");

        file << value_from(settings) << '\n';

        file.close();
    }



    template <class T>
    void extract(const object& obj, T& t, string_view key)
    {
        t = value_to<T>(obj.at(key));
    }



    //--------------------------------------------------------------------------
    // Serialization and deserialize for "TcpClientSettings"
    //--------------------------------------------------------------------------

    // Overloaded for serialization
    void tag_invoke(value_from_tag, value& val, const TcpClientSettings& settings)
    {
        val = 
        {
            { "protocol", settings.protocol},
            { "port", settings.port }
        };
    }

    // Overloaded for deserialize
    TcpClientSettings tag_invoke(value_to_tag<TcpClientSettings>, const value& val)
    {
        TcpClientSettings settings;

        const object& obj = val.as_object();

        extract(obj, settings.protocol, "protocol");
        extract(obj, settings.port, "port");

        return settings;
    }

    //--------------------------------------------------------------------------

} // Distance