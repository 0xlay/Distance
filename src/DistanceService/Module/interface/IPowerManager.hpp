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
// File: IPowerManager.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#pragma once
#include <Windows.h>
#include <new>

namespace Distance::Module
{

#ifndef _POWER_MANAGER_IMPLEMENTATION_

    namespace _PowerManagerInternal
    {

        struct Deleter;

    } // _PowerManagerInternal

#endif // _POWER_MANAGER_IMPLEMENTATION_ 

    struct IPowerManager
    {
        [[nodiscard]] virtual bool shutdown() = 0;
        [[nodiscard]] virtual bool reboot() = 0;
        virtual ~IPowerManager() = default;

    private:
#ifndef _POWER_MANAGER_IMPLEMENTATION_

        friend _PowerManagerInternal::Deleter;

#endif // _POWER_MANAGER_IMPLEMENTATION_ 

        virtual void destruct() = 0;
    };

#ifndef _POWER_MANAGER_IMPLEMENTATION_

    namespace _PowerManagerInternal
    {

        using PowerManagerAPI = IPowerManager* (*)(void);

        struct Deleter
        {
            void operator()(IPowerManager* manager)
            {
                manager->destruct();
            }
        };

    } // _PowerManagerInternal

#endif // _POWER_MANAGER_IMPLEMENTATION_ 

} // Distance::Module
