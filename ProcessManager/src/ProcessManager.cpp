#include "ProcessManager.hpp"

using namespace Distance;
using namespace boost::json;


void tag_invoke(value_from_tag, value& val, const PROCESSENTRY32& process);



void ProcessManager::terminate(DWORD pid)
{
    const xstar::SmartHandle process(OpenProcess(PROCESS_ALL_ACCESS, false, pid));
    if (!process.getHandle())
        throw xstar::WinAPIException();

    TerminateProcess(process.getHandle(), 0);
}



ProcessList& ProcessManager::update()
{
    PROCESSENTRY32 processEntry_{ sizeof(PROCESSENTRY32) };
    const xstar::SmartHandle snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

    processes_->clear();

    if (!Process32First(snapshot.getHandle(), &processEntry_))
        throw xstar::WinAPIException();

    do
    {
        processes_->push_back(processEntry_);
    } while (Process32Next(snapshot.getHandle(), &processEntry_));

    return *processes_;
}



std::string Distance::ProcessListPackToJson(const ProcessList& processList)
{
    return serialize(value_from(processList));
}



// Overloaded for serialization
void tag_invoke(value_from_tag, value& val, const PROCESSENTRY32& process)
{
    val =
    {
        {"dwSize", process.dwSize},
        {"cntUsage", process.cntUsage},
        {"th32ProcessID", process.th32ProcessID},
        {"th32DefaultHeapID", process.th32DefaultHeapID},
        {"th32ModuleID", process.th32ModuleID},
        {"cntThreads", process.cntThreads},
        {"th32ParentProcessID", process.th32ParentProcessID},
        {"pcPriClassBase", process.pcPriClassBase},
        {"dwFlags", process.dwFlags},
        {"szExeFile", process.szExeFile}
    };
}
