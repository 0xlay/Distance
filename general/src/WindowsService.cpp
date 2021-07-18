#include "WindowsService.hpp"

using namespace Distance;



WindowsService::WindowsService(std::vector<ServiceTable>& services)
    : services_(services), status_{ 0 }, statusHandle_(nullptr)
{
    services_.push_back({ nullptr, nullptr });
}



WindowsService::WindowsService(ServiceTable& service)
    : services_{ service }, status_{ 0 }, statusHandle_(nullptr)
{
    services_.push_back({ nullptr, nullptr });
}



void WindowsService::start()
{
    if (!StartServiceCtrlDispatcher(reinterpret_cast<SERVICE_TABLE_ENTRY*>(&services_[0])))
    {
        std::stringstream ss;
        ss << "Failed start services... Code: 0x" << std::hex << GetLastError();
        throw std::runtime_error(ss.str());
    }
}



SERVICE_STATUS* WindowsService::initStatus()
{
    return &status_;
}



void WindowsService::registerHandler(LPCTSTR serviceName, LPHANDLER_FUNCTION_EX handlerProc, LPVOID context)
{
    statusHandle_ = RegisterServiceCtrlHandlerEx(serviceName, handlerProc, context);
}



void WindowsService::setStatus()
{
    if (!SetServiceStatus(statusHandle_, &status_))
    {
        std::stringstream ss;
        ss << "Failed set service status... Code: 0x" << std::hex << GetLastError();
        throw std::runtime_error(ss.str());
    }
}



void WindowsService::updateStatus(int checkPoint, int newState)
{
    if (newState >= 0)
        status_.dwCurrentState = newState;

    if (checkPoint < 0)
        ++status_.dwCheckPoint;
    else
        status_.dwCheckPoint = checkPoint;
    setStatus();
}
