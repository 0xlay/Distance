#include "ClientApp.hpp"



_Use_decl_annotations_
int WINAPI wWinMain(
    [[maybe_unused]] HINSTANCE hInstance,
    [[maybe_unused]] HINSTANCE hPrevInstance,
    [[maybe_unused]] LPWSTR lpCmdLine,
    [[maybe_unused]] int nShowCmd
)
{
    auto clientApplication = Distance::ClientApp::create();

    clientApplication->run();

    delete clientApplication;
    return 0;
}
