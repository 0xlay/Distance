#include "ServerApp.hpp"

namespace Distance
{

    ServerApp* ServerApp::app_ = nullptr;



    ServerApp* ServerApp::create()
    {
        return app_ = new ServerApp();
    }



    void ServerApp::run()
    {
        ServerConfigurator configurator("server_settings.json");
        std::vector<TcpServerSettings> addressList;
        try
        {
            configurator.loadSettings(addressList);
        }
        catch (const std::exception& error)
        {
            std::cout << "Server configurator error: " << error.what() << std::endl;
            return;
        }

        mainBuffer_ = std::make_unique<char[]>(MainBufferSize);

        try
        {
            xstar::NamedPipeServer pipeServer(
                _T("\\\\.\\pipe\\DistanceGUIPipeServer"), 
                xstar::PipeDirection::InOut
            );
            pipeServer.connect();

            int query = 0;
            Package package{ nullptr, 0};

            while (true)
            {
                pipeServer.read(query);

                switch (static_cast<Query>(query))
                {
                case Query::ConnectToPC1:
                    pipeServer.write(connectToPc(addressList[0]));
                    break;
                case Query::ConnectToPC2:
                    pipeServer.write(connectToPc(addressList[1]));
                    break;
                case Query::ConnectToPC3:
                    pipeServer.write(connectToPc(addressList[2]));
                    break;
                case Query::ConnectToPC4:
                    pipeServer.write(connectToPc(addressList[3]));
                    break;
                case Query::ConnectToPC5:
                    pipeServer.write(connectToPc(addressList[4]));
                    break;
                case Query::ConnectToPC6:
                    pipeServer.write(connectToPc(addressList[5]));
                    break;

                case Query::GetKeyLog:
                    package = getKeyboardLog();
                    pipeServer.write(package.buffer, static_cast<uint32_t>(package.numberOfBytes));
                    break;

                case Query::GetProcessInfo:
                    package = processUpdate();
                    pipeServer.write(package.buffer, static_cast<uint32_t>(package.numberOfBytes));
                    break;

                case Query::TerminateProcess:
                    processTerminate(
                        pipeServer.read(mainBuffer_, static_cast<uint32_t>(MainBufferSize))
                    );
                    break;

                case Query::GetScreenshot:
                    package = getScreenshot();
                    pipeServer.write(package.buffer, static_cast<uint32_t>(package.numberOfBytes));
                    break;

                case Query::ShutdownPC:
                    server_->sendQuery(Query::ShutdownPC);
                    break;
                case Query::LogoutPC:
                    server_->sendQuery(Query::LogoutPC);
                    break;
                case Query::RebootPC:
                    server_->sendQuery(Query::RebootPC);
                    break;

                case Query::GetNetLog:
                    package = getNetfilterLog();
                    pipeServer.write(package.buffer, static_cast<uint32_t>(package.numberOfBytes));
                    break;
                default:
                    break;
                }
            }
        }
        catch (const std::exception& error)
        {
            std::cout << error.what() << std::endl;
        }
    }



    std::string ServerApp::connectToPc(std::vector<TcpServerSettings>::const_reference address)
    {
        server_.reset(new TcpSyncServer(ioContext_, address.ip, address.port));
        Sleep(100);


        try
        {
            server_->connectToClient();
        }
        catch(const std::exception&)
        {
            return "FAILED TO CONNECT TO THE CLIENT...";
        }


        server_->sendQuery(Query::GetInfoPC);
        server_->dataReceive(mainBuffer_, MainBufferSize);

        ComputerInfo info;
        info.UnpackJson(mainBuffer_.get());

        std::stringstream stream;
        stream
            << "Local name: " << info.getName()
            << "\nIP: " << info.getIP()
            << "\nRAM: " << info.getMemory().TotalMemory / 1024 / 1024 << "mb"
            << "\nCPU: " << info.getCPU().name << std::endl;

        return stream.str();
    }



    Package ServerApp::getKeyboardLog()
    {
        server_->sendQuery(Query::GetKeyLog);
        return { mainBuffer_.get(), server_->dataReceive(mainBuffer_, MainBufferSize) };
    }



    Package ServerApp::getNetfilterLog()
    {
        server_->sendQuery(Query::GetNetLog);
        return { mainBuffer_.get(), server_->dataReceive(mainBuffer_, MainBufferSize) };
    }



    Package ServerApp::getScreenshot()
    {
        server_->sendQuery(Query::GetScreenshot);
        return { mainBuffer_.get(), server_->dataReceive(mainBuffer_, MainBufferSize) };
    }



    Package ServerApp::processUpdate()
    {
        server_->sendQuery(Query::GetProcessInfo);
        return { mainBuffer_.get(), server_->dataReceive(mainBuffer_, MainBufferSize) };
    }



    void ServerApp::processTerminate(size_t bytesForSend) const
    {
        server_->sendQuery(Query::TerminateProcess);
        server_->dataSend(mainBuffer_.get(), bytesForSend);
    }


} // namespace Distance
