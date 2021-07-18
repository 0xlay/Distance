#include "PowerManager.hpp"

using namespace Distance;


void PowerManager::shutdown()
{
    givePrivilege();
#pragma warning(push)
#pragma warning(disable : 28159)
    ExitWindowsEx(EWX_SHUTDOWN, SHTDN_REASON_MAJOR_OTHER);
#pragma warning(pop)
}



void PowerManager::logout()
{
    ExitWindowsEx(EWX_LOGOFF, SHTDN_REASON_MAJOR_OTHER);
}



void PowerManager::reboot()
{
    givePrivilege();
    ExitWindowsEx(EWX_REBOOT, SHTDN_REASON_MAJOR_OTHER);
}



//------------------------------------------------------------------------------
// This method will enable "SeShutdownPrivilege" privilege. It needs for
// reboots and shutdowns PC.
//------------------------------------------------------------------------------

void PowerManager::givePrivilege()
{
    TOKEN_PRIVILEGES tokenPrivileges;
    HANDLE processToken;


    if (!OpenProcessToken(
        GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processToken
    ))
    {
        throw xstar::WinAPIException();
    }


    if (!LookupPrivilegeValue(
        nullptr,
        SE_SHUTDOWN_NAME,
        &tokenPrivileges.Privileges[0].Luid))
    {
        throw xstar::WinAPIException();
    }


    tokenPrivileges.PrivilegeCount = 1;
    tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(
        processToken, 
        FALSE, 
        &tokenPrivileges, 
        0,
        nullptr, 
        nullptr
    ))
    {
        throw xstar::WinAPIException();
    }
}

//------------------------------------------------------------------------------