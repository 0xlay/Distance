#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <variant>


namespace Distance::ProcessManager
{
    using ProcessList = std::vector<PROCESSENTRY32>;
	using ErrorCode = DWORD;

    /*
    * @brief The ProcessTerminate function terminates process by its ID.
    * 
    * @param[in] pid - process identifier
    * 
    * @return 0 if success, otherwise error code
    */
    ErrorCode ProcessTerminate(DWORD pid);
    
    /*
    * @brief The GetProcessList function gets system process list
    * 
    * @return ProcessList if success, otherwise error code. You must check 
    * return value with help the std::holds_alternative<T>(value) function
    */
    std::variant<ProcessList, ErrorCode> GetProcessList();

} // Distance::ProcessManager
