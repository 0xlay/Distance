#ifndef _DISTANCE_COMMUNICATION_HPP_
#define _DISTANCE_COMMUNICATION_HPP_

namespace Distance
{

    enum class Query
    {
        ConnectToPC1     = 1,
        ConnectToPC2     = 2,
        ConnectToPC3     = 3,
        ConnectToPC4     = 4,
        ConnectToPC5     = 5,
        ConnectToPC6     = 6,
        GetInfoPC        = 7,
        GetKeyLog        = 8,
        GetProcessInfo   = 9,
        TerminateProcess = 10,
        GetScreenshot    = 11,
        ShutdownPC       = 12,
        LogoutPC         = 13,
        RebootPC         = 14,
        GetNetLog        = 15
    };

} // Distance
#endif // _DISTANCE_COMMUNICATION_HPP_