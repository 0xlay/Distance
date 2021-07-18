#ifndef _DISTANCE_POWER_MANAGER_HPP_
#define _DISTANCE_POWER_MANAGER_HPP_

#ifdef BLOCKER_EXPORTS
#define BLOCKER_API __declspec(dllexport)
#else
#define BLOCKER_API __declspec(dllimport)
#endif

#include <Windows.h>
#include "Exception.hpp"

namespace Distance
{

    class BLOCKER_API PowerManager
    {
    public:
        static void shutdown();
        static void logout();
        static void reboot();
    private:
        static void givePrivilege();
    };

} // Distance


#endif // _DISTANCE_POWER_MANAGER_HPP_
