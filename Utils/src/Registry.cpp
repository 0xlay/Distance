#include "Registry.hpp"

namespace Distance::Utils
{

    Registry::Registry(HKEY rootKey, RegistryOption option, RegistryAccess access) 
    : rootKey_(rootKey), subKey_(nullptr), option_(option), access_(access)
    { }

    ErrorCode Registry::openKey(std::wstring_view subKey)
    {
        if (subKey_) 
            closeKey();
        
        return RegOpenKeyExW(
            rootKey_,
            subKey.data(),
            0,
            static_cast<DWORD>(access_),
            &subKey_
        );
    }

    ErrorCode Registry::addKey(std::wstring_view subKey)
    {
        if (subKey_)
            closeKey();

        return RegCreateKeyExW(
            rootKey_,
            subKey.data(),
            0,
            REG_OPTION_NON_VOLATILE, /* The info stored in a file */
            static_cast<DWORD>(option_),
            static_cast<DWORD>(access_),
            nullptr,
            &subKey_,
            nullptr
        );
    }

    ErrorCode Registry::renameKey(std::wstring_view subKey, std::wstring_view newSubKey)
    {
        return RegRenameKey(rootKey_, subKey.data(), newSubKey.data());
    }

    ErrorCode Registry::deleteKey(std::wstring_view subKey)
    {
        return RegDeleteKeyExW(rootKey_, subKey.data(), KEY_WOW64_64KEY, 0);
    }

    ErrorCode Registry::closeKey()
    {
        return RegCloseKey(subKey_);
    }

    ErrorCode Registry::getData(uint32_t& data, std::wstring_view fieldName)
    {
        DWORD sizeInBytes = sizeof(uint32_t);
        return RegQueryValueExW(
            subKey_,
            static_cast<LPCWSTR>(fieldName.data()),
            nullptr,
            nullptr,
            reinterpret_cast<LPBYTE>(&data),
            &sizeInBytes
        );
    }

    ErrorCode Registry::getData(uint64_t& data, std::wstring_view fieldName)
    {
        DWORD sizeInBytes = sizeof(uint64_t);
        return RegQueryValueExW(
            subKey_,
            fieldName.data(),
            nullptr,
            nullptr,
            reinterpret_cast<LPBYTE>(&data),
            &sizeInBytes
        );
    }
    
    ErrorCode Registry::getData(std::vector<uint8_t>& data, std::wstring_view fieldName)
    {
        data.resize(1024);
        DWORD sizeInBytes = data.size();
        LSTATUS status = RegQueryValueExW(
            subKey_,
            fieldName.data(),
            nullptr,
            nullptr,
            reinterpret_cast<LPBYTE>(&data[0]),
            &sizeInBytes
        );
        data.resize(sizeInBytes);
        return status;
    }
    
    ErrorCode Registry::getData(std::wstring& data, std::wstring_view fieldName)
    {
        data.resize(512);
        DWORD sizeInBytes = data.size() * sizeof(wchar_t);
        LSTATUS status = RegQueryValueExW(
            subKey_,
            fieldName.data(),
            nullptr,
            nullptr,
            reinterpret_cast<LPBYTE>(&data[0]),
            &sizeInBytes
        );
        data.resize((sizeInBytes / sizeof(wchar_t)) + 1);
        return status;
    }
    
    ErrorCode Registry::setData(uint32_t data, std::wstring_view fieldName)
    {
        return RegSetValueExW(
            subKey_,
            fieldName.data(),
            0,
            REG_DWORD,
            reinterpret_cast<LPBYTE>(&data),
            sizeof(uint32_t)
        );
    }

    ErrorCode Registry::setData(uint64_t data, std::wstring_view fieldName)
    {
        return RegSetValueExW(
            subKey_,
            fieldName.data(),
            0,
            REG_QWORD,
            reinterpret_cast<LPBYTE>(&data),
            sizeof(uint64_t)
        );
    }

    ErrorCode Registry::setData(const std::vector<uint8_t>& data, std::wstring_view fieldName)
    {
        return RegSetValueExW(
            subKey_,
            fieldName.data(),
            0,
            REG_BINARY,
            &data[0],
            data.size()
        );
    }

    ErrorCode Registry::setData(const std::wstring& data, std::wstring_view fieldName)
    {
        return RegSetValueExW(
            subKey_,
            fieldName.data(),
            0,
            REG_SZ,
            reinterpret_cast<const BYTE*>(&data[0]),
            (data.size() * sizeof(wchar_t)) /* size in bytes */
        );
    }

    ErrorCode Registry::deleteField(std::wstring_view fieldName)
    {
        return RegDeleteValueW(subKey_, fieldName.data());
    }

    Registry::~Registry()
    {
        closeKey();
    }

} // Distance::Utils