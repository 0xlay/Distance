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
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "ClientNetwork.hpp"
#include "App.hpp"
#include "named_pipe.hpp"
#include "Media.hpp"


using namespace Distance;
using namespace Utils;
using Distance::ClientApp;


void ServiceMain(int argc, wchar_t** argv);
uint32_t ServiceCtrlHandler(uint32_t control,uint32_t eventType,
                            void* eventData, void* context);
void ServiceDestructor();


int _tmain()
{
    Service::invoke().create(
        ServiceControl::Stop | ServiceControl::Shutdown | ServiceControl::PowerEvent,
        ServiceType::Win32OwnProcess,
        Constant::serviceName,
        ServiceMain
    );
    return 0;
}



void ServiceMain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t** argv)
{
    Logger::invoke().configure(std::make_unique<LoggerFileStream>(
        (std::wostringstream() << Service::invoke().baseDirectory() << L"logs\\").str()
    ));
    LOG_DEBUG_INFO("[DEBUG MODE]: Entrance in ServiceMain!");

    LOG_INFO("Changing directory !");
    if (!SetCurrentDirectoryW(Service::invoke().baseDirectory().data()))
    {
        LOG_INFO(
            "Failed to change directory! Error code: ", ::GetLastError()
        );
        return;
    }

    LOG_INFO("Registering handler, and set status as StartPending!");
    Service::invoke().registerHandler(ServiceCtrlHandler);
    Service::invoke().update(ServiceStatus::StartPending, 1000);

    try
    {
        LOG_INFO("Opening storage with settings");
        SettingsController::invoke().openStorage(
            StorageType::JsonFile,
            L"client_settings.json"
        );

        LOG_INFO("Running client!");
        ClientApp::getInstance().run();
    }
    catch (const std::exception& error)
    {
        LOG_INFO(error.what());
        ClientApp::getInstance().stop();
        Service::invoke().update(ServiceStatus::Stopped);
    }

    LOG_DEBUG_INFO("[DEBUG MODE]: Exit from ServiceMain!");
}



uint32_t ServiceCtrlHandler(uint32_t control, uint32_t eventType, 
                            [[maybe_unused]] void* eventData, 
                            [[maybe_unused]] void* context)
{
    LOG_DEBUG_INFO("[DEBUG MODE]: Entrance in ServiceCtrlHandler!");
    switch (control)
    {
        case SERVICE_CONTROL_SHUTDOWN:
        case SERVICE_CONTROL_STOP:
        {
            ServiceDestructor();
            return NO_ERROR;
        }
        case SERVICE_CONTROL_POWEREVENT:
        {
            if (eventType == PBT_APMQUERYSUSPEND)
            {
                ServiceDestructor();
            }
            return NO_ERROR;
        }
        default: {}
    }

    LOG_DEBUG_INFO("[DEBUG MODE]: Exit from ServiceCtrlHandler with code:"
                   " ERROR_CALL_NOT_IMPLEMENTED!");
    Service::invoke().update(ServiceStatus::Running);
    return ERROR_CALL_NOT_IMPLEMENTED;
}



void ServiceDestructor()
{
    Service::invoke().update(ServiceStatus::StopPending, 1000);

    try
    {
        xstar::NamedPipeClient pipeClient(Media::g_PipeName,
                                          xstar::PipeAccess::InOut);
        pipeClient.connect();
        pipeClient.write(static_cast<int>(Media::Command::Close));
        pipeClient.disconnect();
    }
    catch (const std::exception& ex)
    {
        LOG_INFO("The named pipe: ", ex.what());
    }

    ClientApp::getInstance().stop();

    LOG_INFO("The Distance service was stopped!");

    Service::invoke().update(ServiceStatus::Stopped);
}
