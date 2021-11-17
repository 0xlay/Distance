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
    }

} // namespace Distance
