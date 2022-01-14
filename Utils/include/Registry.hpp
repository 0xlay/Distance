#pragma once

#include <Windows.h>
#include <string_view>
#include <vector>
#include <functional>
#include <thread>

#include "Type.hpp"

namespace Distance::Utils
{
    
    enum class RegistryOption : uint32_t
    {
        BackupRestore   = REG_OPTION_BACKUP_RESTORE,
        DontVirtualize  = REG_OPTION_DONT_VIRTUALIZE,
        CreateLink      = REG_OPTION_CREATE_LINK,
        OpenLink        = REG_OPTION_OPEN_LINK,
        NonVolatile     = REG_OPTION_NON_VOLATILE,
        Volatile        = REG_OPTION_VOLATILE
    };

    enum class RegistryAccess : uint32_t
    {
        AllAccess       = KEY_ALL_ACCESS,
        CreateLink      = KEY_CREATE_LINK,
        CreateSubKey    = KEY_CREATE_SUB_KEY,
        EnumerateSubKey = KEY_ENUMERATE_SUB_KEYS,
        QueryValue      = KEY_QUERY_VALUE,
        Execute         = KEY_EXECUTE,
        Notify          = KEY_NOTIFY,
        SetValue        = KEY_SET_VALUE,
        Read            = KEY_READ,
        Write           = KEY_WRITE,
        Wow64_32        = KEY_WOW64_32KEY,
        Wow64_64        = KEY_WOW64_64KEY   
    };

    enum class RegNotifyFilter : uint32_t
    {
        ChangeName       = REG_NOTIFY_CHANGE_NAME,
        ChangeAttributes = REG_NOTIFY_CHANGE_ATTRIBUTES,
        LastSet          = REG_NOTIFY_CHANGE_LAST_SET,
        ChangeSecurity   = REG_NOTIFY_CHANGE_SECURITY,
    };

    class Registry
    {
    public:
        Registry(HKEY rootKey, RegistryOption option, RegistryAccess access);

        Registry(const Registry&) = delete;
        Registry(Registry&&) = delete;

        Registry& operator=(const Registry&) = delete;
        Registry& operator=(Registry&&) = delete;

        [[nodiscard]] ErrorCode openKey(std::wstring_view subKey);
        [[nodiscard]] ErrorCode addKey(std::wstring_view subKey);
        [[nodiscard]] ErrorCode renameKey(std::wstring_view subKey, std::wstring_view newSubKey);
        [[nodiscard]] ErrorCode deleteKey(std::wstring_view subKey);
        ErrorCode closeKey();

        [[nodiscard]] ErrorCode getData(uint32_t& data, std::wstring_view fieldName);
        [[nodiscard]] ErrorCode getData(uint64_t& data, std::wstring_view fieldName);
        [[nodiscard]] ErrorCode getData(std::vector<uint8_t>& data, std::wstring_view fieldName);
        [[nodiscard]] ErrorCode getData(std::wstring& data, std::wstring_view fieldName);

        [[nodiscard]] ErrorCode setData(uint32_t data, std::wstring_view fieldName);
        [[nodiscard]] ErrorCode setData(uint64_t data, std::wstring_view fieldName);
        [[nodiscard]] ErrorCode setData(const std::vector<uint8_t>& data, std::wstring_view fieldName);
        [[nodiscard]] ErrorCode setData(const std::wstring& data, std::wstring_view fieldName);

        [[nodiscard]] ErrorCode deleteField(std::wstring_view fieldName);
        
        template <typename ... Args>
        [[nodiscard]] ErrorCode setNotifyChangeData(
            RegNotifyFilter filter,
            bool watchSubtree,
            std::function<void(HANDLE, Args ...)> callback,
            Args ...args
        );

        ~Registry();

    private:
        HKEY rootKey_, subKey_;
        RegistryOption option_;
        RegistryAccess access_;
    };



    template <typename ... Args>
    ErrorCode Registry::setNotifyChangeData(
        RegNotifyFilter filter,
        bool watchSubtree,
        std::function<void(HANDLE, Args ...)> callback,
        Args ... args
    )
    {
        HANDLE event = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        if (!event)
        {
            return GetLastError();
        }
        
        LSTATUS errorCode = RegNotifyChangeKeyValue(
            subKey_,
            watchSubtree,
            static_cast<DWORD>(filter),
            event,
            TRUE
        );
        if (errorCode)
        {
            return errorCode;
        }

        std::thread waitableThread(callback, event, args...);
        waitableThread.detach();
        return 0;
    }

} // Distance::Utils