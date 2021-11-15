#include "ClientNetwork.hpp"
#include "ClientSetting.hpp"


using namespace boost::json;


namespace Distance::Network
{

	ClientSetting::ClientSetting(std::string_view ip, std::uint16_t port)
		: settings_{ ip.data(), port }
	{ }

	void ClientSetting::load(std::fstream& file)
	{
		std::string json_string;
		file >> json_string;

		value json = parse(json_string);
		settings_ = value_to<_internal::ClientSettingPack>(json);
	}

	void ClientSetting::save(std::fstream& file)
	{
		file << value_from(settings_) << '\n';
	}

	void ClientSetting::setIp(std::string_view ip)
	{
		settings_.ip = ip;
	}

	void ClientSetting::setPort(std::uint16_t port)
	{
		settings_.port = port;
	}

	std::string_view ClientSetting::getIp() const
	{
		return settings_.ip;
	}

	std::uint16_t ClientSetting::getPort() const
	{
		return settings_.port;
	}



	//--------------------------------------------------------------------------
	// Serialization and deserialize for "ClientSettings"
	//--------------------------------------------------------------------------

	namespace _internal
	{
		template <class T>
		void extract(const object& obj, T& t, string_view key)
		{
			t = value_to<T>(obj.at(key));
		}

		// Overloaded for serialization
		void tag_invoke(value_from_tag, value& val, const ClientSettingPack& setting)
		{
			val =
			{
				{ "ip", setting.ip},
				{ "port", setting.port }
			};
		}

		// Overloaded for deserialize
		ClientSettingPack tag_invoke(value_to_tag<ClientSettingPack>, const value& json)
		{
			ClientSettingPack settings;
			const object& obj = json.as_object();

			extract(obj, settings.ip, "ip");
			extract(obj, settings.port, "port");

			return settings;
		}
	}

	//--------------------------------------------------------------------------

} // Distance::Network