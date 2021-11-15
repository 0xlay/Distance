#pragma once

#include <Windows.h>


/*
* @brief The PmShutdown function shutdowns computer
* 
* @return true if success, otherwise false
*/
BOOL PmShutdown();

/*
* @brief The PmReboot function reboots computer
* 
* @return true if success, otherwise false
*/
BOOL PmReboot();

/*
* @brief The PmLogout function lefts session by the current user
* 
* @return true if success, otherwise false
*/
BOOL PmLogout();

/*
* @brief The GivePrivelege function, enables SeShutdownPrivilege privilege
* (needs for reboot and shutdown)
*
* @return 0 if success, otherwise error code
*/
DWORD PmGivePrivilege();
