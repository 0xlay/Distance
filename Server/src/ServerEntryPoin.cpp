#include "ServerApp.hpp"



int main()
{
    auto ServerApplication = Distance::ServerApp::create();

    ServerApplication->run();

    delete ServerApplication;
    return 0;
}