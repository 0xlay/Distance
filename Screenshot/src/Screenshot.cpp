#include "Screenshot.hpp"

using namespace Distance;

Screenshot::Screenshot()
    : Bitmap_(nullptr), Screen_(nullptr), Context_(nullptr), bitmapInfo_(nullptr)
{ }



void Screenshot::create(int x1, int y1, int x2, int y2)
{
    const int width = x2 - x1;
    const int height = y2 - y1;

    Screen_ = GetDC(nullptr);
    if (!Screen_)
        throw xstar::WinAPIException();

    Context_ = CreateCompatibleDC(Screen_);
    if (!Context_)
        throw xstar::WinAPIException();

    Bitmap_ = CreateCompatibleBitmap(Screen_, width, height);
    if (!Bitmap_)
        throw xstar::WinAPIException();

    if (!SelectObject(Context_, Bitmap_))
        throw xstar::WinAPIException();

    BOOL isSuccess = 
    BitBlt(
        Context_, 
        0, 0, width, height, 
        Screen_, 
        x1, y1, 
        SRCCOPY
    );
    if (!isSuccess)
        throw xstar::WinAPIException();

    createBitmapInfoStruct();
}



std::string Screenshot::getBMP()
{
    const auto bitmapInfoHeader = reinterpret_cast<PBITMAPINFOHEADER>(bitmapInfo_);

    const auto memory = static_cast<LPBYTE>(GlobalAlloc(GMEM_FIXED, bitmapInfoHeader->biSizeImage));
    if (!memory)
        throw xstar::WinAPIException();

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
        throw xstar::WinAPIException();


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

    std::stringstream ss;

    ss.write(reinterpret_cast<char*>(&bitmapFileHeader), sizeof(BITMAPFILEHEADER));

    ss.write(reinterpret_cast<char*>(bitmapInfoHeader), sizeof(BITMAPINFOHEADER)
        + bitmapInfoHeader->biClrUsed * sizeof(RGBQUAD));

    ss.write(reinterpret_cast<char*>(memory), bitmapInfoHeader->biSizeImage);

    GlobalFree(memory);
    return ss.str();
}



void Screenshot::createBitmapInfoStruct()
{
    BITMAP bmp;

    if (!GetObject(Bitmap_, sizeof(BITMAP), &bmp))
        throw xstar::WinAPIException();

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
        return;

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
}