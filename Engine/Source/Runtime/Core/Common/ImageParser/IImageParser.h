#ifndef CORE_COMMON_IMAGEPARSER_IMAGEPARSER_H
#define CORE_COMMON_IMAGEPARSER_IMAGEPARSER_H
#include "Core/Object/IImage.h"
namespace ReiToEngine
{
template <typename T>
class IImageParser
{
public:
    bool Read(const char* filename, Image& image)
    {
        return static_cast<T*>(this)->ReadImpl(filename, image);
    }
    bool Write(const char* filename,const Image& image, const bool vflip=true, const bool rle=true)
    {   printf("test2\n");
        return static_cast<T*>(this)->WriteImpl(filename, image, vflip, rle);
    }
protected:
    virtual bool ReadImpl(const char* filename, Image& image) = 0;
    virtual bool WriteImpl(const char* filename,const Image& image, const bool vflip, const bool rle) const = 0; 
};
}
#endif