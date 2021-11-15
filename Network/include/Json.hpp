#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include <cassert>


namespace Distance::Network::Json
{
	/*
	* @brief The PcInfo struct keeps information about of RAM, CPU, local name, and local IP
	*/
	struct PcInfo
	{
		struct
		{
			DWORD MemoryLoad{};      /*percent*/
			DWORDLONG TotalMemory{}; /*bytes*/
			DWORDLONG AvailMemory{}; /*bytes*/
		}ram;
		struct
		{
			std::string architecture;
			std::string name;
		} cpu;
		std::string localName;
		std::string ip;
	};

	/*
	* @brief The ProcessList keeps info about of processes
	*/
	using ProcessList = std::vector<PROCESSENTRY32>;



	//
	// Unsupported types
	//

	/*
	* @brief The pack function converts data type to JSON format
	*/
	template <typename T>
	std::string pack(const T& object)
	{
		static_assert(false, "This type is not supported!");
	}

	/*
	* @brief The unpack function converts from JSON format to data type
	*/
	template <typename T>
	void unpack(const std::string& jsonString, T& object)
	{
		static_assert(false, "This type is not supported!");
	}



	//
	// Explicit template specialization for PcInfo
	//

	template <>
	std::string pack(const PcInfo& info);
	
	template <>
	void unpack(const std::string& jsonString, PcInfo& info);



	//
	// Explicit template specialization for ProcessList
	//

	template <>
	std::string pack(const ProcessList& list);

	template <>
	void unpack(const std::string& jsonString, ProcessList& list);

} // Distance::Network::Json
