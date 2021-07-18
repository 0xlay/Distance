#ifndef _DISTANCE_SCREENSHOT_APP_HPP_
#define _DISTANCE_SCREENSHOT_APP_HPP_

#include <Windows.h>
#include <string>
#include <sstream>
#include "Exception.hpp"


#ifdef SCREENSHOT_EXPORTS
#define SCREENSHOT_API __declspec(dllexport)
#else
#define SCREENSHOT_API __declspec(dllimport)
#endif


namespace Distance
{

    class SCREENSHOT_API Screenshot
    {
    public:
        Screenshot();

        void create(
            int x1 = GetSystemMetrics(SM_XVIRTUALSCREEN), 
            int y1 = GetSystemMetrics(SM_YVIRTUALSCREEN), 
            int x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN), 
            int y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN)
        );

        std::string getBMP();

    private:
        void createBitmapInfoStruct();

    private:
        HBITMAP Bitmap_;
        HDC Screen_;
        HDC Context_;
        PBITMAPINFO bitmapInfo_;

        enum
        {
            BM = 0x4d42
        };
    };

    extern SCREENSHOT_API std::string CreateScreenshot();

} // Distance
#endif // _DISTANCE_SCREENSHOT_APP_HPP_
