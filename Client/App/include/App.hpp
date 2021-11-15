#pragma once

// #include <iostream>
// #include <cwctype>
// #include <cassert>
// #include <codecvt>

#include <Windows.h>
#include <memory>

#include "DefaultTcpClient.hpp"
#include "ClientSetting.hpp"
#include "ClientHandler.hpp"
#include "ModulesManager.hpp"
// #include "ComputerInfo.hpp"
#include "Logger.hpp"
#include "tstring.hpp"


namespace Distance
{

    class ClientApp
    {
    public:
        ClientApp(const ClientApp&) = delete;
        ClientApp(ClientApp&&) = delete;
        ClientApp& operator=(const ClientApp&) = delete;
        ClientApp& operator=(ClientApp&&) = delete;

        static ClientApp& getInstance();
        void run();

    private:
        ClientApp() = default;
        ~ClientApp() = default;
    };

} // namespace Distance
