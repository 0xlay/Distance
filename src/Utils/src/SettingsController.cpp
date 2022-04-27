//------------------------------------------------------------------------------
// ╔═══╗╔══╗╔═══╗╔════╗╔═══╗╔═╗─╔╗╔═══╗╔═══╗
// ╚╗╔╗║╚╣─╝║╔═╗║║╔╗╔╗║║╔═╗║║║╚╗║║║╔═╗║║╔══╝
// ─║║║║─║║─║╚══╗╚╝║║╚╝║║─║║║╔╗╚╝║║║─╚╝║╚══╗
// ─║║║║─║║─╚══╗║──║║──║╚═╝║║║╚╗║║║║─╔╗║╔══╝
// ╔╝╚╝║╔╣─╗║╚═╝║──║║──║╔═╗║║║─║║║║╚═╝║║╚══╗
// ╚═══╝╚══╝╚═══╝──╚╝──╚╝─╚╝╚╝─╚═╝╚═══╝╚═══╝
// 
// Copyright Ⓒ 0xlay. All rights reserved.
// 
// This source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
// 
// File: SettingsController.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Utils.lib
//
//------------------------------------------------------------------------------

#include "SettingsController.hpp"
#include <regex>
#include <filesystem>

#define __LOG_TAG__ "[SettingsController]:"

struct RegPath
{
    HKEY rootKey;
    std::wstring subKey;
};

[[nodiscard]] RegPath PathToRegPath(std::wstring_view path);

namespace Distance::Utils
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // SettingsController implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    SettingsController& SettingsController::invoke()
    {
        std::lock_guard lock(mutex_);
        if (!controller_)
        {
            controller_.reset(new SettingsController());
        }
        return *controller_;
    }

    void SettingsController::openStorage(StorageType storage, std::wstring_view path)
    {
        std::lock_guard lock(mutex_);
        storage_ = storage;
        path_ = path;
        switch(storage_)
        {
            case StorageType::JsonFile:
            {
                openFile();
                break;
            }
            case StorageType::Registry:
            {
                openRegistry();
                break;
            }
        }
    }

    void SettingsController::save(std::shared_ptr<Settings> settings)
    {
        std::lock_guard lock(mutex_);
        switch(storage_)
        {
            case StorageType::JsonFile:
            {
                // clear file
                std::filesystem::resize_file(path_, 0);
                settings->saveToFile(file_);
                break;
            }
            case StorageType::Registry:
            {
                settings->saveToRegistry(registry_);
                break;
            }
        }
    }

    void SettingsController::load(std::shared_ptr<Settings> settings)
    {
        std::lock_guard lock(mutex_);
        switch(storage_)
        {
            case StorageType::JsonFile:
            {
                if (!std::filesystem::is_empty(path_))
                {
                    settings->loadFromFile(file_);
                }
                else
                {
                    // save default settings
                    settings->saveToFile(file_);
                }
                break;
            }
            case StorageType::Registry:
            {
                try
                {
                    settings->loadFromRegistry(registry_);
                }
                catch (const _internal::RegDataNotFoundException&)
                {
                    // save default settings
                    settings->saveToRegistry(registry_);
                }
                break;
            }
        }
    }

    //
    // Private methods
    //

    void SettingsController::openFile()
    {
        file_ = std::make_unique<std::wfstream>(path_, std::ios::in | std::ios::out | std::ios::app);
        if (!file_->is_open())
        {
            throw std::runtime_error(__LOG_TAG__"Failed to open or create file!");
        }
    }

    void SettingsController::openRegistry()
    {
        ::RegPath regPath = ::PathToRegPath(path_);
        registry_ = std::make_unique<Registry>(
            regPath.rootKey,
            RegistryOption::NonVolatile,
            RegistryAccess::AllAccess
        );
        // Creates key, or open if key already exists
        if (registry_->addKey(regPath.subKey))
        {
            throw std::runtime_error(__LOG_TAG__"Failed to open or create key in registry!");
        }
    }

    namespace _internal
    {

        ////////////////////////////////////////////////////////////////////////
        //
        // SaveFileHelper implementation
        //
        ////////////////////////////////////////////////////////////////////////

        SaveFileHelper::SaveFileHelper(std::shared_ptr<std::wfstream> file)
            : file_(std::move(file))
        {}

        void SaveFileHelper::save(const std::string& data)
        {
#pragma warning(push)
#pragma warning (disable : 4996)
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            file_->clear(); // clear flags
            file_->seekp(0, std::ios::beg);
            *file_ << converter.from_bytes(data);
            file_->flush();
#pragma warning(pop)

        }

        void SaveFileHelper::save(const std::wstring& data)
        {
            file_->clear(); // clear flags
            file_->seekp(0, std::ios::beg);
            *file_ << data;
            file_->flush();
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // LoadFileHelper implementation
        //
        ////////////////////////////////////////////////////////////////////////

        LoadFileHelper::LoadFileHelper(std::shared_ptr<std::wfstream> file)
            : file_(std::move(file))
        {}

        void LoadFileHelper::load(std::string& data)
        {
#pragma warning(push)
#pragma warning (disable : 4996)
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring temp;
            file_->seekg(0, std::ios::beg);
            std::getline(*file_, temp);
            data = converter.to_bytes(temp);
#pragma warning(pop)
        }

        void LoadFileHelper::load(std::wstring& data)
        {
            file_->seekg(0, std::ios::beg);
            std::getline(*file_, data);
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // SaveRegistryHelper implementation
        //
        ////////////////////////////////////////////////////////////////////////

        SaveRegistryHelper::SaveRegistryHelper(std::shared_ptr<Registry> registry)
            : registry_(std::move(registry))
        {}

        void SaveRegistryHelper::save(uint32_t data, std::wstring_view fieldName)
        {
            if (registry_->setData(data, fieldName))
                throw std::runtime_error(__LOG_TAG__"Failed to save data in registry!");
        }

        void SaveRegistryHelper::save(uint64_t data, std::wstring_view fieldName)
        {
            if (registry_->setData(data, fieldName))
                throw std::runtime_error(__LOG_TAG__"Failed to save data in registry!");
        }

        void SaveRegistryHelper::save(const std::vector<uint8_t>& data, std::wstring_view fieldName)
        {
            if (registry_->setData(data, fieldName))
                throw std::runtime_error(__LOG_TAG__"Failed to save data in registry!");
        }

        void SaveRegistryHelper::save(const std::string& data, std::wstring_view fieldName)
        {
#pragma warning(push)
#pragma warning(disable : 4996)
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            if (registry_->setData(converter.from_bytes(data), fieldName))
                throw std::runtime_error(__LOG_TAG__"Failed to save data in registry!");
#pragma warning(pop)
        }

        void SaveRegistryHelper::save(const std::wstring& data, std::wstring_view fieldName)
        {
            if (registry_->setData(data, fieldName))
                throw std::runtime_error(__LOG_TAG__"Failed to save data in registry!");
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // LoadRegistryHelper implementation
        //
        ////////////////////////////////////////////////////////////////////////

        LoadRegistryHelper::LoadRegistryHelper(std::shared_ptr<Registry> registry)
            : registry_(std::move(registry))
        {}

        void LoadRegistryHelper::load(uint32_t& data, std::wstring_view fieldName)
        {
            if (LSTATUS error = registry_->getData(data, fieldName); error == ERROR_FILE_NOT_FOUND)
            {
                throw RegDataNotFoundException();
            }
            else if (error)
            {
                throw std::runtime_error(__LOG_TAG__"Failed to load data from registry!");
            }
        }

        void LoadRegistryHelper::load(uint64_t& data, std::wstring_view fieldName)
        {
            if (LSTATUS error = registry_->getData(data, fieldName); error == ERROR_FILE_NOT_FOUND)
            {
                throw RegDataNotFoundException();
            }
            else if (error)
            {
                throw std::runtime_error(__LOG_TAG__"Failed to load data from registry!");
            }
        }

        void LoadRegistryHelper::load(std::vector<uint8_t>& data, std::wstring_view fieldName)
        {
            if (LSTATUS error = registry_->getData(data, fieldName); error == ERROR_FILE_NOT_FOUND)
            {
                throw RegDataNotFoundException();
            }
            else if (error)
            {
                throw std::runtime_error(__LOG_TAG__"Failed to load data from registry!");
            }
        }

        void LoadRegistryHelper::load(std::string& data, std::wstring_view fieldName)
        {
#pragma warning(push)
#pragma warning(disable : 4996)
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring res;

            if (LSTATUS error = registry_->getData(res, fieldName); error == ERROR_FILE_NOT_FOUND)
            {
                throw RegDataNotFoundException();
            }
            else if (error)
            {
                throw std::runtime_error(__LOG_TAG__"Failed to load from registry!");
            }

            data = converter.to_bytes(res);
#pragma warning(pop)
        }

        void LoadRegistryHelper::load(std::wstring& data, std::wstring_view fieldName)
        {
            if (LSTATUS error = registry_->getData(data, fieldName); error == ERROR_FILE_NOT_FOUND)
            {
                throw RegDataNotFoundException();
            }
            else if (error)
            {
                throw std::runtime_error(__LOG_TAG__"Failed to load data from registry!");
            }
        }

    } // Distance::Utils::_internal

} // Distance::Utils


RegPath PathToRegPath(std::wstring_view path)
{
    RegPath regPath{};
    std::wregex wregx(L"(\\[\\w+\\])");
    std::wcmatch match;

    std::regex_search(path.data(), match, wregx);

    if (match[0] == L"[HKEY_CLASSES_ROOT]")
    {
        regPath.rootKey = HKEY_CLASSES_ROOT;
        regPath.subKey = match.suffix();
    }
    else if (match[0] == L"[HKEY_CURRENT_CONFIG]")
    {
        regPath.rootKey = HKEY_CURRENT_CONFIG;
        regPath.subKey = match.suffix();
    }
    else if (match[0] == L"[HKEY_CURRENT_USER]")
    {
        regPath.rootKey = HKEY_CURRENT_USER;
        regPath.subKey = match.suffix();
    }
    else if (match[0] == L"[HKEY_LOCAL_MACHINE]")
    {
        regPath.rootKey = HKEY_LOCAL_MACHINE;
        regPath.subKey = match.suffix();
    }
    else if (match[0] == L"[HKEY_USERS]")
    {
        regPath.rootKey = HKEY_USERS;
        regPath.subKey = match.suffix();
    }

    return regPath;
}
