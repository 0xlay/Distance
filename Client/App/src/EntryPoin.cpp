#include "ClientNetwork.hpp"
#include "App.hpp"

using Distance::Network::Configurator;
using Distance::Utils::Logger;
using Distance::ClientApp;
using Distance::Utils::LoggerFileStream;

_Use_decl_annotations_
int WINAPI wWinMain(
    [[maybe_unused]] HINSTANCE hInstance,
    [[maybe_unused]] HINSTANCE hPrevInstance,
    [[maybe_unused]] LPWSTR lpCmdLine,
    [[maybe_unused]] int nShowCmd
)
{
    Logger::invoke().configure(std::make_unique<LoggerFileStream>($T"logs/"));
    try
    {
        Configurator::invoke().open($T"client_settings.json");
        ClientApp::getInstance().run();
    }
    catch (const std::exception& error)
    {
        LOG_INFO("The ClientApp failed! Error message: ", error.what())
    }
    return 0;
}
