#include "PowerManager.hpp"
#include <new>


BOOL PmShutdown()
{
	DWORD errorCode = PmGivePrivilege();
	if (errorCode)
		return FALSE;

#pragma warning(push)
#pragma warning(disable : 28159)
	return ExitWindowsEx(EWX_SHUTDOWN, SHTDN_REASON_MAJOR_OTHER);
#pragma warning(pop)
}

BOOL PmReboot()
{
	DWORD errorCode = PmGivePrivilege();
	if (errorCode)
		return FALSE;

	return ExitWindowsEx(EWX_REBOOT, SHTDN_REASON_MAJOR_OTHER);
}

BOOL PmLogout()
{
	return ExitWindowsEx(EWX_LOGOFF, SHTDN_REASON_MAJOR_OTHER);
}



DWORD PmGivePrivilege()
{
	TOKEN_PRIVILEGES tokenPrivileges;
	HANDLE processToken;


	if (!OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processToken
	))
	{
		return GetLastError();
	}


	if (!LookupPrivilegeValue(
		nullptr,
		SE_SHUTDOWN_NAME,
		&tokenPrivileges.Privileges[0].Luid
	))
	{
		return GetLastError();
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
		return GetLastError();
	}

	return 0; // Success
}
