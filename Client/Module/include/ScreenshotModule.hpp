#pragma once

#include "Module.hpp"
#include <variant>


namespace Distance::Module
{

    class Screenshot : public IModule
    {
		using ErrorCode = DWORD;
		using Bitmap = std::string;

        using CreateScreenshotFunc = std::variant<Bitmap, ErrorCode>(*)(int coordX, int coordY, int widthX, int heightY);
        using CreateFullScreenshotFunc = std::variant<Bitmap, ErrorCode>(*)();

    public:
        ~Screenshot();
        
        [[nodiscard]] Bitmap createScreenshot(int coordX, int coordY, int widthX, int heightY);
        [[nodiscard]] Bitmap createFullScreenshot();

        void run() override;
        void stop() override;

    private:
        HMODULE hModule{};

        CreateScreenshotFunc CreateScreenshot;
        CreateFullScreenshotFunc CreateFullScreenshot;
    };

} // Distance::Module
