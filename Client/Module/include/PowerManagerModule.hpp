#pragma once

#include "Module.hpp"


namespace Distance::Module
{

    class PowerManager : public IModule
    {
        using PmFunc = BOOL(*)();

    public:
        ~PowerManager();

        [[nodiscard]] bool shutdown();
        [[nodiscard]] bool reboot();
        [[nodiscard]] bool logout();

        void run() override;
        void stop() override;

    private:
        HMODULE hModule{};

        PmFunc PmShutdown, PmReboot, PmLogout;
    };

} // Distance::Module
