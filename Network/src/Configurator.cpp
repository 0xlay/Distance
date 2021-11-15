#include "Network.hpp"
#include "Configurator.hpp"


namespace Distance::Network
{

	namespace _internal
	{
		void ConfiguratorDestructor::init(Configurator* configurator)
		{
			configurator_ = configurator;
		}

		ConfiguratorDestructor::~ConfiguratorDestructor()
		{
			delete configurator_;
		}
	}



	Configurator& Configurator::invoke()
	{
		std::lock_guard lock(mutex_);
		if (!configurator_)
		{
			configurator_ = new Configurator();
			destructor_.init(configurator_);
		}
		return *configurator_;
	}

	void Configurator::open(xstar::tstring_view path)
	{
		path_ = path;
	}

	void Configurator::loadSettings(Settings* settings)
	{
		std::lock_guard lock(mutex_);
		if (file_.is_open()) 
			file_.close();

		file_.open(path_, std::ios::in | std::ios::out | std::ios::app);
		if (!file_.is_open())
			throw std::runtime_error("File with settings didn't opened.");

		if (!isFileEmpty(file_))
		{
			settings->load(file_);
		}
		else
		{
			file_.close();
			file_.open(path_, std::ios::in | std::ios::out | std::ios::trunc);
			if (!file_.is_open())
				throw std::runtime_error("File with settings didn't opened.");

			settings->save(file_);
			file_.flush();
		}
	}

	void Configurator::saveSettings(Settings* settings)
	{
		std::lock_guard lock(mutex_);
		if (file_.is_open()) 
			file_.close();

		file_.open(path_, std::ios::in | std::ios::out | std::ios::trunc);
		if (!file_.is_open())
			throw std::runtime_error("File with settings didn't opened.");

		settings->save(file_);
		file_.flush();
	}

} // Distance::Network::Network