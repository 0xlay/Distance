#pragma once

#include <Windows.h>
#include <exception>
#include <stdexcept>
#include <string>

namespace Distance::Module
{
    
    struct IModule
    {
        virtual void run() = 0;
        virtual void stop() = 0;

        virtual ~IModule() = default;
    };

} // Distance::Module
