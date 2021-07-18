#include "ServerConfigurator.hpp"

using namespace boost::json;

namespace Distance
{
    ServerConfigurator::ServerConfigurator(const char* path)
        : path_(path)
    { }



    void ServerConfigurator::loadSettings(std::vector<TcpServerSettings>& settings) const
    {
        std::ifstream file_(path_);
        if (!file_.is_open())
            throw std::runtime_error("Ifstream: failed opened file...");
        
        std::string jsonString;
        value val;

        while (!file_.eof())
        {
            file_ >> jsonString;
            
            val = parse(jsonString);

            settings.push_back(value_to<TcpServerSettings>(val));
        }
    }



    void ServerConfigurator::saveSettings(const std::vector<TcpServerSettings>& settings) const
    {
        std::ofstream file_(path_);
        if (!file_.is_open())
            throw std::runtime_error("Ofstream: failed opened file...");

        for (const auto& setting : settings)
            file_ << value_from(setting) << '\n';
    }



    template <class T>
    void extract(const object& obj, T& t, string_view key)
    {
        t = value_to<T>(obj.at(key));
    }



    //--------------------------------------------------------------------------
    // Serialization and deserialize for "TcpServerSettings"
    //--------------------------------------------------------------------------

    // Overloaded for serialization
    void tag_invoke(value_from_tag, value& val, const TcpServerSettings& setting)
    {
        val =
        {
            { "ip", setting.ip},
            { "port", setting.port }
        };
    }

    // Overloaded for deserialize
    TcpServerSettings tag_invoke(value_to_tag<TcpServerSettings>, const value& val)
    {
        TcpServerSettings settings;
        const object& obj = val.as_object();

        extract(obj, settings.ip, "ip");
        extract(obj, settings.port, "port");

        return settings;
    }

    //--------------------------------------------------------------------------

} // Distance