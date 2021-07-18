#ifndef _DISTANCE_NETFILTER_CONTROL_HPP_
#define _DISTANCE_NETFILTER_CONTROL_HPP_

#include <Windows.h>
#include <fwpmu.h>

#include <memory>
#include <string>

#include "Exception.hpp"
#include "StartWinDriver.hpp"
#include "NativeAPI.hpp"

#define NETFILTER_LOGFILE_NAME "logs\\netlog.txt"


// {C6E41CF6-2749-4382-9268-50AC2335CB1F}
static const GUID NETFILTER_ACTION_CALLOUT_SPYING_AND_TERMINATING =
{ 0xc6e41cf6, 0x2749, 0x4382, { 0x92, 0x68, 0x50, 0xac, 0x23, 0x35, 0xcb, 0x1f } };

// {CE52B6AA-F205-4C13-ADE0-A89C306655EE}
static const GUID NETFILTER_SUB_LAYER =
{ 0xce52b6aa, 0xf205, 0x4c13, { 0xad, 0xe0, 0xa8, 0x9c, 0x30, 0x66, 0x55, 0xee } };


namespace Distance
{
    class NetfilterControl
    {
    public:
        NetfilterControl(const char* path);
        ~NetfilterControl();

        char* getInfo();

    private:
        void InitWfp();
        void UnInitWfp();

    private:
        HANDLE Logfile_;
        HANDLE Engine_;
        uint32_t calloutAddId_;
        uint64_t filterId_;

        std::unique_ptr<char[]> buffer_;
        std::unique_ptr<xstar::StartWinDriver> driverCallout_;
    };
} // Distance


#endif // _DISTANCE_NETFILTER_CONTROL_HPP_