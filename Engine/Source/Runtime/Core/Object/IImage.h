#ifndef CORE_OBJECT_IIMAGE_H
#define CORE_OBJECT_IIMAGE_H
#include "Core/HAL/Macro/Macro.h"
#include <cstddef>
#include <cstdio>
#include <memory>
#include <cstring>
namespace ReiToEngine
{

struct IColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

enum EImageType
{
    IMAGE_UNKNOWN       = 0x00,
    IMAGE_TGA			= 0x01,
    IMAGE_BMP           = 0x02,
	IMAGE_PNG           = 0x04,
    IMAGE_JPEG          = 0x08,
};

struct ImageInfo
{
    EImageType imageType;
    int w = 0;
    int h = 0;
    int channels = 0;
};

class RTENGINE_API IImage {
public:
    virtual ~IImage() = 0;
    virtual bool SetData(uint8_t* data) = 0;
    virtual bool SetData(uint8_t* data, ImageInfo image_info) = 0;
    virtual const uint8_t* GetData() const = 0;
    virtual const ImageInfo& GetConstImageInfo() const = 0;
    virtual void FlipHorizontally() = 0;
    virtual void FlipVertically() = 0;
    virtual IColor GetColor(const int x, const int y) const = 0;
    virtual void SetColor(const int x, const int y, const IColor& c) = 0;
    RT_FORCEINLINE int GetWidth() const
    {
        return imageInfo.w;
    }
    RT_FORCEINLINE int GetHeight() const
    {
        return imageInfo.h;
    }
protected:
    ImageInfo imageInfo;
    uint8_t* _data = nullptr;
};
inline ReiToEngine::IImage::~IImage() = default;

class RTENGINE_API Image : public IImage
{
friend class IImage;
public:
    Image(int width, int height, int channels)
    {
        imageInfo.w = width;
        imageInfo.h = height;
        imageInfo.channels = channels;
        imageInfo.imageType = EImageType::IMAGE_UNKNOWN;
        _data = new uint8_t[width * height * channels];
    }
    ~Image()
    {
        delete [] _data;
        _data = nullptr;
    }
    bool SetData(uint8_t* data) override
    {
        if (_data) {
            delete[] _data;
        }

        if (!data) {
            _data = nullptr;
            return false;
        }

        size_t dataSize = imageInfo.w * imageInfo.h * imageInfo.channels;
        printf("datasize%zd\n", dataSize);
        _data = new uint8_t[dataSize];
        memcpy(_data, data, dataSize);
        return true;
    }
    void SetType(EImageType type)
    {
        imageInfo.imageType = type;
    }
    bool SetData(uint8_t* data, ImageInfo image_info) override
    {
        imageInfo = image_info;
        return SetData(data);
    }
    const uint8_t* GetData() const override
    {
        return _data;
    }
    const ImageInfo& GetConstImageInfo() const override{
        return imageInfo;
    }

    void FlipHorizontally() override
    {
        if (!_data || imageInfo.w <= 0 || imageInfo.h <= 0)
            return;

        const int rowSize = imageInfo.w * imageInfo.channels;
        for (int y = 0; y < imageInfo.h; y++)
        {
            uint8_t *row = _data + y * rowSize;
            for (int x = 0; x < imageInfo.w / 2; x++)
            {
                const int left = x * imageInfo.channels;
                const int right = (imageInfo.w - 1 - x) * imageInfo.channels;
                for (int c = 0; c < imageInfo.channels; c++)
                {
                    std::swap(row[left + c], row[right + c]);
                }
            }
        }
    }

    void FlipVertically() override
    {
        if (!_data || imageInfo.w <= 0 || imageInfo.h <= 0)
            return;

        const int rowSize = imageInfo.w * imageInfo.channels;
        uint8_t temp;

        for (int y = 0; y < imageInfo.h / 2; y++)
        {
            uint8_t *topRow = _data + y * rowSize;
            uint8_t *bottomRow = _data + (imageInfo.h - 1 - y) * rowSize;

            for (int x = 0; x < rowSize; x++)
            {
                temp = topRow[x];
                topRow[x] = bottomRow[x];
                bottomRow[x] = temp;
            }
        }
    }
    IColor GetColor(const int w, const int h) const override
    {
        IColor color = {0, 0, 0, 255};
        if (!_data || w < 0 || w >= imageInfo.w || h < 0 || h >= imageInfo.h)
            return color;

        const int offset = (h * imageInfo.w + w) * imageInfo.channels;
        color.r = _data[offset];
        color.g = imageInfo.channels > 1 ? _data[offset + 1] : color.r;
        color.b = imageInfo.channels > 2 ? _data[offset + 2] : color.g;
        color.a = imageInfo.channels > 3 ? _data[offset + 3] : 255;
        return color;
    }
    void SetColor(const int w, const int h, const IColor& c) override
    {
        if (!_data || w < 0 || w >= imageInfo.w || h < 0 || h >= imageInfo.h)
            return;

        const int offset = (h * imageInfo.w + w) * imageInfo.channels;
        _data[offset] = c.r;
        if (imageInfo.channels > 1) _data[offset + 1] = c.g;
        if (imageInfo.channels > 2) _data[offset + 2] = c.b;
        if (imageInfo.channels > 3) _data[offset + 3] = c.a;
    }
};

}
#endif
