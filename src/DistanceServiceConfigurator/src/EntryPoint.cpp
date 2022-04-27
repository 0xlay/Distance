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
// File: EntryPoint.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceServiceConfigurator.exe
//
//------------------------------------------------------------------------------

#include <iostream>
#include <memory>
#include "Configurator.hpp"
#include "Constant.hpp"
#include "Logo.hpp"

using Distance::ErrorCode;

int main()
{
    std::unique_ptr<Distance::ServiceConfigurator> service;
    try
    {
        service.reset(new Distance::ServiceConfigurator(Distance::Constant::serviceName));
    }
    catch(const std::exception& error)
    {
        std::cout << error.what() << std::endl;
        system("pause");
        return 0;
    }
    uint32_t var = 0;

    std::cout << Distance::g_Logo << std::endl;
    std::cout << "1. Install\n2. Uninstall\n3. Run\n4. Stop\n5. Exit" << std::endl;

    while(true)
    {
        std::cout << ">>";
        std::cin >> var;
        switch (var)
        {
            case 1:
            {
                if (ErrorCode error = service->install(); error)
                {
                    std::cout << "Service wasn't installed! Error code: " << error << std::endl;
                }
                else
                {
                    std::cout << "Service was installed!" << std::endl;
                }
                break;
            }
            case 2:
            {
                if (ErrorCode error = service->uninstall(); error)
                {
                    std::cout << "Service wasn't uninstalled! Error code: " << error << std::endl;
                }
                else
                {
                    std::cout << "Service was uninstalled!" << std::endl;
                }
                break;
            }
            case 3:
            {
                if (ErrorCode error = service->run(); error)
                {
                    std::cout << "Service wasn't run! Error code: " << error << std::endl;
                }
                else
                {
                    std::cout << "Service was run!" << std::endl;
                }
                break;
            }
            case 4:
            {
                if (ErrorCode error = service->stop(); error)
                {
                    std::cout << "Service wasn't stopped! Error code: " << error << std::endl;
                }
                else
                {
                    std::cout << "Service was stopped!" << std::endl;
                }
                break;
            }
            case 5:
            {
                return 0;
            }
            default:
            {
                std::cout << "Incorrect option!" << std::endl;
                break;
            }
        }
    }
}
