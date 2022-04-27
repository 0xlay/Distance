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
// File: SettingsController.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: Utils.lib
//
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <fstream>
#include <mutex>
#include <codecvt>
#include <cassert>
#include "Registry.hpp"


namespace Distance::Utils
{

    namespace _internal
    {

        ////////////////////////////////////////////////////////////////////////
        //
        // File helpers
        //
        ////////////////////////////////////////////////////////////////////////

        class SaveFileHelper final
        {
        public:
            SaveFileHelper(std::shared_ptr<std::wfstream> file);
            void save(const std::string& data);
            void save(const std::wstring& data);

        private:
            std::shared_ptr<std::wfstream> file_;
        };

        class LoadFileHelper final
        {
        public:
            LoadFileHelper(std::shared_ptr<std::wfstream> file);
            void load(std::string& data);
            void load(std::wstring& data);

        private:
            std::shared_ptr<std::wfstream> file_;
        };

        ////////////////////////////////////////////////////////////////////////
        //
        // Registry helpers
        //
        ////////////////////////////////////////////////////////////////////////

        class SaveRegistryHelper final
        {
        public:
            SaveRegistryHelper(std::shared_ptr<Registry> registry);

            void save(uint32_t data, std::wstring_view fieldName);
            void save(uint64_t data, std::wstring_view fieldName);
            void save(const std::vector<uint8_t>& data, std::wstring_view fieldName);
            void save(const std::string& data, std::wstring_view fieldName);
            void save(const std::wstring& data, std::wstring_view fieldName);

        private:
            std::shared_ptr<Registry> registry_;
        };

        class LoadRegistryHelper final
        {
        public:
            LoadRegistryHelper(std::shared_ptr<Registry> registry);

            void load(uint32_t& data, std::wstring_view fieldName);
            void load(uint64_t& data, std::wstring_view fieldName);
            void load(std::vector<uint8_t>& data, std::wstring_view fieldName);
            void load(std::string& data, std::wstring_view fieldName);
            void load(std::wstring& data, std::wstring_view fieldName);

        private:
            std::shared_ptr<Registry> registry_;
        };

        struct RegDataNotFoundException : std::exception
        {
            const char* what() const override
            { return "The data was not found in the registry!"; }
        };

    } // _internal



    struct Settings
    {
        virtual void saveToFile(_internal::SaveFileHelper file)
        {
            assert(false && "You didn't override Settings::saveToFile()");
        }

        virtual void loadFromFile(_internal::LoadFileHelper file)
        {
            assert(false && "You didn't override Settings::loadFromFile()");
        }

        virtual void saveToRegistry(_internal::SaveRegistryHelper registry)
        {
            assert(false && "You didn't override Settings::saveToRegistry()");
        }

        virtual void loadFromRegistry(_internal::LoadRegistryHelper registry)
        {
            assert(false && "You didn't override Settings::loadFromRegistry()");
        }

        virtual ~Settings() = 0;
    };
    inline Settings::~Settings() = default;

    enum class StorageType
    {
        Registry,
        JsonFile
    };

    /*
    * @brief The SettingsController class implements the interface for working with settings
    */
    class SettingsController final
    {
    public:
        SettingsController(const SettingsController&) = delete;
        SettingsController(SettingsController&&) = delete;

        SettingsController& operator=(const SettingsController&) = delete;
        SettingsController& operator=(SettingsController&&) = delete;

        [[maybe_unused]] static SettingsController& invoke();

        /*
        * @brief Opens a storage for keeping data
        *
        * @param[in] storage - storage type (JsonFile, Registry)
        *
        * @param[in] path - file path, registry path ("[HKEY_CURRENT_USER]Software\\...")
        */
        void openStorage(StorageType storage,  std::wstring_view path);

        /*
        * @brief Saves settings in an opened storage
        *
        * @brief[in] settings - any settings which inheritance "Settings" interface
        */
        void save(std::shared_ptr<Settings> settings);

        /*
        * @brief Loads settings from an opened storage
        *
        * @brief[in] settings - any settings which inheritance "Settings" interface
        */
        void load(std::shared_ptr<Settings> settings);

        ~SettingsController() = default;

    private:
        SettingsController() = default;

        void openFile();
        void openRegistry();

    private:
        static inline std::unique_ptr<SettingsController> controller_{};
        static inline std::mutex mutex_;
        StorageType storage_{};
        std::wstring path_;

        std::shared_ptr<std::wfstream> file_;
        std::shared_ptr<Registry> registry_;
    };

} // Distance::Utils
