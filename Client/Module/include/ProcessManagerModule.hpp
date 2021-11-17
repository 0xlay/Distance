#pragma once

#include "Module.hpp"
#include <Tlhelp32.h>
#include <vector>
#include <variant>


namespace Distance::Module
{
    /*
    * @brief The ProcessManager class it is interface for control ProcessManager module
    */
    class ProcessManager : public IModule
    {
		using ProcessList = std::vector<PROCESSENTRY32>;
		using ErrorCode = DWORD;

        using ProcessTerminateFunc = ErrorCode(*)(DWORD);
        using GetProcessListFunc = std::variant<ProcessList, ErrorCode>(*)();

    public:
        ~ProcessManager();

        [[nodiscard]] void terminate(DWORD pid);
        [[nodiscard]] ProcessList processList();

        void run() override;
        void stop() override;

    private:
        HMODULE hModule{};

        ProcessTerminateFunc ProcessTerminate;
        GetProcessListFunc GetProcessList;
    };

} // Distance::Module
