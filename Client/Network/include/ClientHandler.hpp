#pragma once

#include "Client.hpp"
#include "Logger.hpp"

namespace Distance::Network::Handler
{

    void Connect(
        std::shared_ptr<Client> client,
        Message& msg,
        const boost::system::error_code& ec
    );

    void Receive(
        std::shared_ptr<Client> client,
        Message& msg,
        const boost::system::error_code& ec,
        size_t size
    );

} // Distance::Network::Handler
