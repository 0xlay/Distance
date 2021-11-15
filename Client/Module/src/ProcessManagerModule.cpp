#include "ProcessManagerModule.hpp"


namespace Distance::Module
{

	ProcessManager::~ProcessManager()
	{
		FreeLibrary(hModule);
	}

	void ProcessManager::terminate(DWORD pid)
	{
		ErrorCode code = ProcessTerminate(pid);
		if (code)
			throw std::runtime_error("Failed to terminate process! Error code: " + std::to_string(code));
	}

	ProcessManager::ProcessList ProcessManager::processList()
	{
		auto list = GetProcessList();
		if (std::holds_alternative<ErrorCode>(list))
			throw std::runtime_error("Failed to have process list! Error code: " + std::to_string(std::get<ErrorCode>(list)));
		
		return std::get<ProcessList>(list);
	}

	void ProcessManager::run()
	{
		hModule = LoadLibrary(TEXT("ProcessManager.dll"));
		if (!hModule)
			throw std::runtime_error(
				"Failed to load the ProcessManager library! Error code: " + std::to_string(GetLastError())
			);

		ProcessTerminate = reinterpret_cast<ProcessTerminateFunc>(GetProcAddress(hModule, "ProcessTerminate"));
		GetProcessList = reinterpret_cast<GetProcessListFunc>(GetProcAddress(hModule, "GetProcessList"));

		if (!ProcessTerminate || !GetProcessList)
			throw std::runtime_error("Failed to get a valid procedure address!");
	}

	void ProcessManager::stop()
	{
		FreeLibrary(hModule);
	}

} // Distance::Module