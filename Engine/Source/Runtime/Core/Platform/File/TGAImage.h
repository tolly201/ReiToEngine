#ifndef CORE_PLATFORM_FILE_TGAIMAGE_H
#define CORE_PLATFORM_FILE_TGAIMAGE_H

#include "IImage.h"

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

#endif