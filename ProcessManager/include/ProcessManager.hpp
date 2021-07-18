#ifndef _DISTANCE_PROCESS_MANAGER_HPP_
#define _DISTANCE_PROCESS_MANAGER_HPP_

#include <Windows.h>
#include <TlHelp32.h>

#include <vector>
#include <string>

#include "SmartHandle.hpp"
#include "Exception.hpp"

#include <boost/json.hpp>

#ifdef PROCESS_MANAGER_EXPORTS
#define PROCESS_MANAGER_API __declspec(dllexport)
#else
#define PROCESS_MANAGER_API __declspec(dllimport)
#endif


namespace Distance
{
    using ProcessList = std::vector<PROCESSENTRY32>;

    class PROCESS_MANAGER_API ProcessManager
    {
    public:
        ProcessManager() = delete;
        ~ProcessManager() = delete;

        static ProcessList& update();
        static void terminate(DWORD pid);

    private:
        inline static ProcessList* processes_ = new ProcessList();
    };

    std::string PROCESS_MANAGER_API ProcessListPackToJson(const ProcessList& processList);


} // Distance

#endif // _DISTANCE_PROCESS_MANAGER_HPP_
