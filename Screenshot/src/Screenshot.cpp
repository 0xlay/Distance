#include "Screenshot.hpp"

namespace Distance
{
    Screenshot::Screenshot()
        : Bitmap_(nullptr), Screen_(nullptr), Context_(nullptr), bitmapInfo_(nullptr)
    { }

    DWORD Screenshot::create(int coordX, int coordY, int widthX, int heightY)
    {
        const int width = widthX - coordX;
        const int height = heightY - coordY;

        // Retrieve a handle to a device context 
        Screen_ = GetDC(nullptr);
        if (!Screen_)
            return GetLastError();

        // Create the device context in memory
        Context_ = CreateCompatibleDC(Screen_);
        if (!Context_)
            return GetLastError();

        // Create bitmap
        Bitmap_ = CreateCompatibleBitmap(Screen_, width, height);
        if (!Bitmap_)
            return GetLastError();

        // Select bitmap
        if (!SelectObject(Context_, Bitmap_))
            return GetLastError();

        // Copy image
        BOOL isSuccess =
            BitBlt(
                Context_,
                0, 0, width, height,
                Screen_,
                coordX, coordY,
                SRCCOPY
            );
        if (!isSuccess)
            return GetLastError();

        DWORD isError = createBitmapInfoStruct();
        if (isError)
        {
            return isError;
        }

        return 0;
    }

    std::variant<Bitmap, ErrorCode> Screenshot::getImage()
    {
        const auto bitmapInfoHeader = reinterpret_cast<PBITMAPINFOHEADER>(bitmapInfo_);

        const auto memory = static_cast<LPBYTE>(GlobalAlloc(GMEM_FIXED, bitmapInfoHeader->biSizeImage));
        if (!memory)
            return GetLastError();

        // Retrieve the bits of the specified compatible bitmap and copies them into a buffer,
        // as a DIB using the specified format
        auto isSuccess =
            GetDIBits(
                Context_,
                Bitmap_,
                0,
                bitmapInfoHeader->biHeight,
                memory,
                bitmapInfo_,
                DIB_RGB_COLORS
            );
        if (!isSuccess)
            return GetLastError();


        BITMAPFILEHEADER bitmapFileHeader{
            BM, // The file type; must be BM. 0x42 = "B" 0x4d = "M"
            static_cast<DWORD>
            (
                sizeof(BITMAPFILEHEADER) + bitmapInfoHeader->biSize + bitmapInfoHeader->biClrUsed
                * sizeof(RGBQUAD) + bitmapInfoHeader->biSizeImage
            ),
            0,
            0,
            static_cast<DWORD>
            (
                sizeof(BITMAPFILEHEADER) + bitmapInfoHeader->biSize +
                bitmapInfoHeader->biClrUsed * sizeof(RGBQUAD)
            )
        };


        //
        // Saving to string stream, images in BMP format
        //
        size_t infoHeaderSize = sizeof(BITMAPINFOHEADER) + bitmapInfoHeader->biClrUsed * sizeof(RGBQUAD);
        Bitmap bmp(sizeof(BITMAPFILEHEADER) + infoHeaderSize + bitmapInfoHeader->biSizeImage);

        memcpy(bmp.data(), &bitmapFileHeader, sizeof(BITMAPFILEHEADER));
        memcpy(bmp.data() + sizeof(BITMAPFILEHEADER), bitmapInfoHeader, infoHeaderSize);
        memcpy(bmp.data() + sizeof(BITMAPFILEHEADER) + infoHeaderSize, memory, bitmapInfoHeader->biSizeImage);

        GlobalFree(memory);
        return bmp;
    }

    DWORD Screenshot::createBitmapInfoStruct()
    {
        BITMAP bmp;

        // Retrieves information for the specified graphics object
        if (!GetObject(Bitmap_, sizeof(BITMAP), &bmp))
            return GetLastError();

        // Convert the color format to a count of bits.  
        auto countBits = static_cast<WORD>(bmp.bmPlanes * bmp.bmBitsPixel);


        if (countBits == 1) { countBits = 1; }

        else if (countBits <= 4) { countBits = 4; }

        else if (countBits <= 8) { countBits = 8; }

        else if (countBits <= 16) { countBits = 16; }

        else if (countBits <= 24) { countBits = 24; }

        else { countBits = 32; }


        if (countBits < 24)
        {
            bitmapInfo_ =
                static_cast<PBITMAPINFO>(
                    LocalAlloc(
                        LPTR,
                        sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) *
                        (static_cast<size_t>(1u) << countBits)
                    )
                    );
        }
        else
        {
            bitmapInfo_ =
                static_cast<PBITMAPINFO>(
                    LocalAlloc(
                        LPTR,
                        sizeof(BITMAPINFOHEADER)
                    )
                    );
        }

        if (!bitmapInfo_)
            return GetLastError();

        bitmapInfo_->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bitmapInfo_->bmiHeader.biWidth = bmp.bmWidth;
        bitmapInfo_->bmiHeader.biHeight = bmp.bmHeight;
        bitmapInfo_->bmiHeader.biPlanes = bmp.bmPlanes;
        bitmapInfo_->bmiHeader.biBitCount = bmp.bmBitsPixel;

        if (countBits < 24)
        {
            bitmapInfo_->bmiHeader.biClrUsed = (1 << countBits);
        }

        bitmapInfo_->bmiHeader.biCompression = BI_RGB;

        bitmapInfo_->bmiHeader.biSizeImage =
            ((bitmapInfo_->bmiHeader.biWidth * countBits + 31) & ~31) /
            8 * bitmapInfo_->bmiHeader.biHeight;

        bitmapInfo_->bmiHeader.biClrImportant = 0;

        return 0;
    }



    std::variant<Bitmap, ErrorCode> CreateScreenshot(int coordX, int coordY, int widthX, int heightY)
    {
        // Set DPI
        if (!SetProcessDPIAware())
        {
            return GetLastError();
        }
        
        Screenshot screenshot;
        DWORD isError = screenshot.create(coordX, coordY, widthX, heightY);
        if (isError)
        {
            return isError;
        }

        // Get image in BMP format
        return screenshot.getImage();
    }

    std::variant<Bitmap, ErrorCode> CreateFullScreenshot()
    {
        return CreateScreenshot(
            GetSystemMetrics(SM_XVIRTUALSCREEN),
            GetSystemMetrics(SM_YVIRTUALSCREEN),
            GetSystemMetrics(SM_CXVIRTUALSCREEN),
            GetSystemMetrics(SM_CYVIRTUALSCREEN)
        );
    }

} // Distance