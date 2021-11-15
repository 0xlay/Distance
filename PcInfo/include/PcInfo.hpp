#pragma once

#include <Windows.h>
#include <string>
#include <variant>


namespace Distance::PcInfo
{
	using ErroCode = DWORD;

	/*
	* @brief The Ram structure stores information about of RAM
	*/
	struct Ram
	{
		DWORD MemoryLoad;			/*in percents*/
		DWORDLONG TotalMemory;		/*in bytes*/
		DWORDLONG AvailMemory;		/*in bytes*/
	};

	/*
	* @brief The Cpu structure stores information about of CPU
	*/
	struct Cpu
	{
		std::string architecture;	/*x64, x86, ...*/
		std::string name;			/*full name: Intel(R) Core(TM) i5-3320M CPU @ 2.60 GHz*/
	};



	/*
	* @brief The GetRamInfo function gets information about of RAM
	*/
	std::variant<Ram, ErroCode> GetRamInfo();
	
	/*
	* @brief The GetCpuInfo function gets information about of CPU
	*/
	std::variant<Cpu, ErroCode> GetCpuInfo();
	
	/*
	* @brief The GetLocalName function gets PC local name
	*/
	std::variant<std::string, ErroCode> GetLocalName();

	/*
	* @brief The GetLocalIp function gets local IP addresses
	*/
	std::variant<std::string, ErroCode> GetLocalIp();

} // Distance::PcInfo
