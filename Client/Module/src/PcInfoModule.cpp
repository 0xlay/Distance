#include "PcInfoModule.hpp"


namespace Distance::Module
{

	PcInfo::~PcInfo()
	{
		FreeLibrary(hModule);
	}

	void PcInfo::run()
	{
		hModule = LoadLibrary(TEXT("PcInfo.dll"));
		if (!hModule)
			throw std::runtime_error(
				"Failed to load the PcInfo library! Error code: " + std::to_string(GetLastError())
			);

		GetRamInfo	 = reinterpret_cast<RamInfoFunc>(GetProcAddress(hModule, "GetRamInfo"));
		GetCpuInfo	 = reinterpret_cast<CpuInfoFunc>(GetProcAddress(hModule, "GetCpuInfo"));
		GetLocalName = reinterpret_cast<LocalNameFunc>(GetProcAddress(hModule, "GetLocalName"));
		GetLocalIp	 = reinterpret_cast<LocalIpFunc>(GetProcAddress(hModule, "GetLocalIp"));

		if (!GetRamInfo || !GetCpuInfo || !GetLocalName || !GetLocalIp)
			throw std::runtime_error("Failed to get a valid procedure address!");
	}

	void PcInfo::stop()
	{
		FreeLibrary(hModule);
	}

	_internal::Ram PcInfo::ram()
	{
		auto ramInfo = GetRamInfo();
		if (std::holds_alternative<ErrorCode>(ramInfo))
			throw std::runtime_error(
				"Failed to get a memory info! Error code: " + std::to_string(std::get<ErrorCode>(ramInfo))
			);

		return std::get<_internal::Ram>(ramInfo);
	}

	_internal::Cpu PcInfo::cpu()
	{
		auto cpuInfo = GetCpuInfo();
		if (std::holds_alternative<ErrorCode>(cpuInfo))
			throw std::runtime_error(
				"Failed to get a cpu info! Error code: " + std::to_string(std::get<ErrorCode>(cpuInfo))
			);
		
		return std::get<_internal::Cpu>(cpuInfo);
	}

	std::string PcInfo::localName()
	{
		auto name = GetLocalName();
		if (std::holds_alternative<ErrorCode>(name))
			throw std::runtime_error(
				"Failed to get a local name! Error code: " + std::to_string(std::get<ErrorCode>(name))
			);

		return std::get<std::string>(name);
	}

	std::string PcInfo::localIp()
	{
		auto ip = GetLocalIp();
		if (std::holds_alternative<ErrorCode>(ip))
			throw std::runtime_error(
				"Failed to get a IP! Error code: " + std::to_string(std::get<ErrorCode>(ip))
			);

		return std::get<std::string>(ip);
	}

} // Distance::Module
