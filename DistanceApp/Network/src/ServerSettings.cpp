#include "pch.hpp"
#include "ServerSettings.hpp"


using namespace boost::json;


namespace Distance::Network
{

	void ServerSettings::save(std::fstream& file)
	{
		file << value_from(settings_) << '\n';
	}

	void ServerSettings::load(std::fstream& file)
	{
		std::string json_string;
		file >> json_string;

		value json = parse(json_string);
		settings_ = value_to<_internal::ServerSettingsPack>(json);
	}

	void ServerSettings::setProtocol(InternetProtocol protocol)
	{
		settings_.protocol = protocol;
	}

	void ServerSettings::setPort(std::uint16_t port)
	{
		settings_.port = port;
	}

	InternetProtocol ServerSettings::getProtocol() const
	{
		return settings_.protocol;
	}

	std::uint16_t ServerSettings::getPort() const
	{
		return settings_.port;
	}



	//--------------------------------------------------------------------------
	// Serialization and deserialize for "ServerSettings"
	//--------------------------------------------------------------------------

	namespace _internal
	{
		template <class T>
		void extract(const object& obj, T& t, string_view key)
		{
			t = value_to<T>(obj.at(key));
		}

		// Overloaded for serialization
		void tag_invoke(value_from_tag, value& val, const ServerSettingsPack& settings)
		{
			std::string protocol_;
			switch (settings.protocol)
			{
			case InternetProtocol::IPV6:
				protocol_ = "IPV6";
				break;
			case InternetProtocol::IPV4:
			default:
				protocol_ = "IPV4";
				break;
			}

			val =
			{
				{ "protocol", std::move(protocol_) },
				{ "port", settings.port }
			};
		}

		// Overloaded for deserialize
		ServerSettingsPack tag_invoke(value_to_tag<ServerSettingsPack>, const value& json)
		{
			ServerSettingsPack settings;
			std::string protocol;
			const object& obj = json.as_object();

			extract(obj, protocol, "protocol");
			extract(obj, settings.port, "port");

			if (protocol == "IPV6")
			{
				settings.protocol = InternetProtocol::IPV6;
			}
			else
			{
				settings.protocol = InternetProtocol::IPV4;
			}

			return settings;
		}
	}

	//--------------------------------------------------------------------------

} // Distance::Network