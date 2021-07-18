#include "Screenshot.hpp"
#include <tchar.h>

#include <iostream>
#include <memory>
#include <sstream>


std::string Distance::CreateScreenshot()
{
    SetProcessDPIAware();
    Distance::Screenshot screenshot;
    screenshot.create();

    return screenshot.getBMP();
}
