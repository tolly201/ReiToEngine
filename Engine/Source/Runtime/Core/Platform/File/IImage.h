#ifndef CORE_PLATFORM_FILE_IIMAGE_H
#define CORE_PLATFORM_FILE_IIMAGE_H
#include "Core/HAL/Platform.h"
#include "CFile.h"

struct IColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

namespace ReiToEngine
{
#pragma pack(push,1)
struct TGAHeader {
    uint8_t  idlength = 0;
    uint8_t  colormaptype = 0;
    uint8_t  datatypecode = 0;
    uint16_t colormaporigin = 0;
    uint16_t colormaplength = 0;
    uint8_t  colormapdepth = 0;
    uint16_t x_origin = 0;
    uint16_t y_origin = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t  bitsperpixel = 0;
    uint8_t  imagedescriptor = 0;
};
#pragma pack(pop)

template<typename T>
class RTENGINE_API IImage {
    virtual IImage() = 0;
    virtual IImage(const int w, const int h, const int bpp) = 0;
    virtual ~IImage = 0;
    virtual bool Read(const char* filename) = 0;
    virtual bool Write(const char* filename, const bool vflip=true, const bool rle=true) const = 0;
    virtual void FlipHorizontally() = 0;
    virtual void FlipVertically() = 0;
    virtual IColor GetColor(const int x, const int y) const = 0;
    virtual void SetColor(const int x, const int y, const IColor& c) = 0;
    virtual void SetColor(const int x, const int y, const IColor&& c) = 0;
    RT_FORCEINLINE int GetWidth() const
    {
        return w;
    };
    RT_FORCEINLINE int GetHeight() const
    {
        return h;
    };
protected:
    int w = 0;
    int h = 0;
    uint8_t bpp = 0;    //bytes per pixel
    uint8_t* data = {};
};

class RTENGINE_API Image : public IImage<Image>
{
friend class IImage<Image>;
};

}
#endif