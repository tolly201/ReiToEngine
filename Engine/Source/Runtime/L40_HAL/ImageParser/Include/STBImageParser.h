#ifndef CORE_COMMON_STBIMAGEPARSER_IMAGEPARSER_H
#define CORE_COMMON_STBIMAGEPARSER_IMAGEPARSER_H
#include "IImageParser.h"
namespace ReiToEngine
{
class RTENGINE_API STBImageParser : public IImageParser<STBImageParser>
{
friend class IImageParser<STBImageParser>;
protected:
    bool ReadImpl(const char* filename, Image& image) override;
    bool WriteImpl(const char* filename, const Image& image, const bool vflip, const bool rle) const override;
};
}
#endif
