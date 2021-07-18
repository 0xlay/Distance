#ifndef _DISTANCE_SERVICE_HPP_
#define _DISTANCE_SERVICE_HPP_

#include <Windows.h>
#include <vector>
#include <stdexcept>
#include <sstream>

namespace Distance
{
    typedef void (WINAPI* SERVICE_ENTRY)(uint32_t, TCHAR**);


    struct ServiceTable
    {
#ifdef _UNICODE
        LPCWSTR       serviceName;
        SERVICE_ENTRY serviceProc;
#else
        LPSTR         serviceName;
        SERVICE_ENTRY serviceProc;
#endif
    };


    class WindowsService
    {
    public:
        WindowsService(ServiceTable& service);
        WindowsService(std::vector<ServiceTable>& services);

        void start();
        SERVICE_STATUS* initStatus();

        void registerHandler(
            LPCTSTR serviceName,
            LPHANDLER_FUNCTION_EX handlerProc,
            LPVOID context
        );
        void setStatus();
        void updateStatus(int checkPoint, int newState = -1);

    private:
        std::vector<ServiceTable> services_;

        SERVICE_STATUS status_;
        SERVICE_STATUS_HANDLE statusHandle_;
    };

} // Distance
#endif // DISTANCE_SERVICE_HPP
