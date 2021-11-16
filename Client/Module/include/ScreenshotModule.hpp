#pragma once

#include "Module.hpp"
#include <variant>
#include <vector>


namespace Distance::Module
{

    class Screenshot : public IModule
    {
		using ErrorCode = DWORD;
		using Bitmap = std::vector<std::byte>;

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
