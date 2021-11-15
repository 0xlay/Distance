#include "PowerManagerModule.hpp"


namespace Distance::Module
{

	PowerManager::~PowerManager()
	{
		FreeLibrary(hModule);
	}

	bool PowerManager::shutdown()
	{
		return PmShutdown();
	}

	bool PowerManager::reboot()
	{
		return PmReboot();
	}

	bool PowerManager::logout()
	{
		return PmLogout();
	}

	void PowerManager::run()
	{
		hModule = LoadLibrary(TEXT("PowerManager.dll"));
		if (!hModule)
			throw std::runtime_error(
				"Failed to load the PowerManager library! Error code: " + std::to_string(GetLastError())
			);
		
		PmShutdown = reinterpret_cast<PmFunc>(GetProcAddress(hModule, "PmShutdown"));
		PmReboot = reinterpret_cast<PmFunc>(GetProcAddress(hModule, "PmReboot"));
		PmLogout = reinterpret_cast<PmFunc>(GetProcAddress(hModule, "PmLogout"));

		if (!PmShutdown || !PmReboot || !PmLogout)
			throw std::runtime_error("Failed to get a valid procedure address!");
	}

	void PowerManager::stop()
	{
		FreeLibrary(hModule);
	}

} // Distance::Module