#include "ClientApp.hpp"

using Distance::Logger;
using Distance::ClientApp;
using Distance::LoggerFileStream;

_Use_decl_annotations_
int WINAPI wWinMain(
    [[maybe_unused]] HINSTANCE hInstance,
    [[maybe_unused]] HINSTANCE hPrevInstance,
    [[maybe_unused]] LPWSTR lpCmdLine,
    [[maybe_unused]] int nShowCmd
)
{
    Logger::make()->configure(std::make_unique<LoggerFileStream>(_T("logs/")));
    ClientApp::getInstance()->run();
    return 0;
}
