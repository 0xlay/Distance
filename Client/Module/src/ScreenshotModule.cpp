#include "ScreenshotModule.hpp"


namespace Distance::Module
{

	Screenshot::~Screenshot()
	{
		FreeLibrary(hModule);
	}

	Screenshot::Bitmap Screenshot::createScreenshot(int coordX, int coordY, int widthX, int heightY)
	{
		auto image = CreateScreenshot(coordX, coordY, widthX, heightY);
		if (std::holds_alternative<ErrorCode>(image))
		{
			throw std::runtime_error("Failed to have screenshot! Error code: " + std::to_string(std::get<ErrorCode>(image)));
		}

		return std::get<Bitmap>(image);
	}

	Screenshot::Bitmap Screenshot::createFullScreenshot()
	{
		auto image = CreateFullScreenshot();
		if (std::holds_alternative<ErrorCode>(image))
		{
			throw std::runtime_error("Failed to have full screenshot! Error code: " + std::to_string(std::get<ErrorCode>(image)));
		}

		return std::get<Bitmap>(image);
	}

	void Screenshot::run()
	{
		hModule = LoadLibrary(TEXT("Screenshot.dll"));
		if (!hModule)
			throw std::runtime_error(
				"Failed to load the Screenshot library! Error code: " + std::to_string(GetLastError())
			);

		CreateScreenshot = reinterpret_cast<CreateScreenshotFunc>(GetProcAddress(hModule, "CreateScreenshot"));
		CreateFullScreenshot = reinterpret_cast<CreateFullScreenshotFunc>(GetProcAddress(hModule, "CreateFullScreenshot"));

		if (!CreateScreenshot || !CreateFullScreenshot)
			throw std::runtime_error("Failed to get a valid procedure address!");
	}

	void Screenshot::stop()
	{
		FreeLibrary(hModule);
	}

} // Distance::Module