#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION // 必须在包含 stb_image.h 之前定义一次
#include "stb_image.h"
#include "stb_image_write.h"

#include "../Include/STBImageParser.h"
#include <iostream>
namespace ReiToEngine
{
bool STBImageParser::ReadImpl(const char* filename, Image& image)
{
    int width, height, channels;
    int format = 0;
    uint8_t* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image: " << filename << "\n";  // 添加错误日志
        return false; // 加载失败
    }

    const char* ext = strrchr(filename, '.');
    EImageType imageType = EImageType::IMAGE_UNKNOWN;
    if (ext) {
        if (strcmp(ext, ".png") == 0) {
            imageType = EImageType::IMAGE_PNG;
        } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
            imageType = EImageType::IMAGE_JPEG;
        } else if (strcmp(ext, ".bmp") == 0) {
            imageType = EImageType::IMAGE_BMP;
        } else if (strcmp(ext, ".tga") == 0) {
            imageType = EImageType::IMAGE_TGA;
        }
    }

    ImageInfo info {
        .imageType = imageType,
        .w = height,
        .h = width,
        .channels = channels  // 修正为实际通道数
    };

    image.SetData(data, info);
    // const ImageInfo c = image.GetConstImageInfo();
    stbi_image_free(data);
    std::cout << "Succeeded to load image: " << filename << "\n";  // 添加错误日志
    // printf("file info:\n height:%d\nwidth:%d\nchannels:%d\n", c.h, c.w, c.channels);
    return true;
}
bool STBImageParser::WriteImpl(const char* filename,const Image& image, const bool vflip, const bool rle) const
{
    printf("test1\n");
    const uint8_t* data = image.GetData();
    printf("test2\n");
    const ImageInfo imageInfo = image.GetConstImageInfo();
    printf("test3\n");
    if (imageInfo.imageType == EImageType::IMAGE_UNKNOWN) {
        std::cerr << "no File Extension\n";
        return false; // 无扩展名
    }

    if (imageInfo.imageType == EImageType::IMAGE_PNG) {
        return stbi_write_png(filename, imageInfo.w, imageInfo.h, imageInfo.channels, data, imageInfo.channels * imageInfo.w);
    } else if (imageInfo.imageType == EImageType::IMAGE_JPEG) {
        return stbi_write_jpg(filename, imageInfo.w, imageInfo.h, imageInfo.channels, data, 90); // 默认质量为 90
    } else if (imageInfo.imageType == EImageType::IMAGE_BMP) {
        return stbi_write_bmp(filename, imageInfo.w, imageInfo.h, imageInfo.channels, data);
    } else if (imageInfo.imageType == EImageType::IMAGE_TGA) {
        return stbi_write_tga(filename, imageInfo.w, imageInfo.h, imageInfo.channels, data);
    }
}
}

