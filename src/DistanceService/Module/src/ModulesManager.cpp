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
// File: ModulesManager.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "ModulesManager.hpp"

namespace Distance
{

    namespace _internal
    {
        void ModulesManagerDestructor::init(Distance::ModulesManager* manager)
        {
            manager_ = manager;
        }

        ModulesManagerDestructor::~ModulesManagerDestructor()
        {
            delete manager_;
        }
    }



    ModulesManager& ModulesManager::invoke()
    {
        if (!manager_)
        {
            manager_ = new ModulesManager();
            destructor_.init(manager_);
        }
        return *manager_;
    }

    void ModulesManager::addModules(ModulesList modules)
    {
        modules_ = std::move(modules);
    }

    ModulesManager::~ModulesManager()
    {
        stopAll();
    }

    void ModulesManager::runAll()
    {
        for (const auto& [key, item] : modules_)
            item->run();
    }

    void ModulesManager::stopAll()
    {
        for (const auto& [key, item] : modules_)
            item->stop();
    }

    void ModulesManager::run(ModuleType mod)
    {
        modules_[mod]->run();
    }

    void ModulesManager::stop(ModuleType mod)
    {
        modules_[mod]->stop();
    }

    std::shared_ptr<Module::IModule> ModulesManager::getAccess(ModuleType mod) const
    {
        return modules_.at(mod);
    }



    ModulesList CreateModules()
    {
        // WARNING: If updated function, change function description in the header file
        ModulesList modules;
        modules[ModuleType::SysInfo]         = std::make_shared<Module::SysInfo>();
        modules[ModuleType::Netfilter]      = std::make_shared<Module::Netfilter>();
        modules[ModuleType::ProcessManager] = std::make_shared<Module::ProcessManager>();
        modules[ModuleType::PowerManager]   = std::make_shared<Module::PowerManager>();
        return modules;
    }

} // Distance