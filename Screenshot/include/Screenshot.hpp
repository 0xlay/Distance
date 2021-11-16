#pragma  once

#include <Windows.h>
#include <vector>
#include <variant>


namespace Distance
{
    using ErrorCode = DWORD;
    using Bitmap = std::vector<std::byte>;

    /*
    * @brief The Screenshot class implements logic for creating a screenshot
    */
    class Screenshot
    {
    public:
        Screenshot();

        DWORD create(int coordX, int coordY, int widthX, int heightY);

        std::variant<Bitmap, ErrorCode> getImage();

    private:
        DWORD createBitmapInfoStruct();

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

    /*
    * @brief The CreateScreenshot function creates a screenshot by coordinates
    * 
    * @param[in] coordX - the coordinates for the left side of the virtual screen. Example: "GetSystemMetrics(SM_XVIRTUALSCREEN)"
    * @param[in] coordY - the coordinates for the top of the virtual screen. Example: "GetSystemMetrics(SM_YVIRTUALSCREEN)"
    * @param[in] widthX - the width of the virtual screen. Example: "GetSystemMetrics(SM_CXVIRTUALSCREEN)"
    * @param[in] heightY - the height of the virtual screen. Example: "GetSystemMetrics(SM_CYVIRTUALSCREEN)"
    * 
	* @return Bitmap if success, otherwise error code. You must check
	* return value with help the std::holds_alternative<T>(value) function
    */
    std::variant<Bitmap, ErrorCode> CreateScreenshot(int coordX, int coordY, int widthX, int heightY);

    /*
    * @brief The CreateFullScreenshot function creates a full screenshot
    * 
	* @return Bitmap if success, otherwise error code. You must check
	* return value with help the std::holds_alternative<T>(value) function
    */
    std::variant<Bitmap, ErrorCode> CreateFullScreenshot();

} // Distance
