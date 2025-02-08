#ifndef FUNCTION_RENDERER_IRENDERER_H
#define FUNCTION_RENDERER_IRENDERER_H

#include "Core/HAL/Platform.h"
#include <cstddef>
#include <vector>
namespace ReiToEngine
{
class RTENGINE_API IRenderer
{
public:
    virtual size_t CreateBuffer() = 0;
    virtual size_t CreateBuffer(uint32_t h, uint32_t w, uint8_t channels) = 0;
    virtual void DrawLine(size_t buffer_object, int x0, int y0, int x1, int y1) = 0;
    virtual std::uint8_t* GetBuffer(size_t buffer_object) = 0;
};

class RTENGINE_API SimpleRenderer : public IRenderer
{
public:
    struct BufferInfo
    {
        u_int32_t height;
        u_int32_t width;
        u_int8_t channels;
    };

    ~SimpleRenderer()
    {
        int size = datas.size();
        for (int i =0; i < size; ++i)
        {
            delete[] datas[i];
        }
        datas.clear();
    }
    size_t CreateBuffer() override
    {
        uint8_t* data = new uint8_t[1];
        datas.push_back(data);

    };
    size_t CreateBuffer(uint32_t w, uint32_t h, uint8_t channels) override
    {
        uint8_t* data = new uint8_t[h * w * channels];
        size_t index = datas.size();
        BufferInfo bi{
            .height = h,
            .width = w,
            .channels = channels
        };
        bufferInfos.push_back(bi);
        datas.push_back(data);
        return index;
    };
    void DrawLine(size_t buffer_object, int x0, int y0, int x1, int y1) override
    {
        uint8_t* data = datas[buffer_object];
        BufferInfo bi = bufferInfos[buffer_object];
        bool steep = false; 
        if (std::abs(x0-x1)<std::abs(y0-y1)) { 
            std::swap(x0, y0); 
            std::swap(x1, y1); 
            steep = true; 
        } 
        if (x0>x1) { 
            std::swap(x0, x1); 
            std::swap(y0, y1); 
        } 
        int dx = x1-x0; 
        int dy = y1-y0; 
        int derror2 = std::abs(dy)*2; 
        int error2 = 0; 
        int y = y0; 
        for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            SetColor(data, bi, y, x); 
        } else { 
            SetColor(data, bi, x, y); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        }
    }
    };

    void SetBackColor(size_t buffer_object, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        uint8_t* data = datas[buffer_object];
        BufferInfo bi = bufferInfos[buffer_object];
        for (int i = 0; i < bi.height;++i)
            for (int j = 0; j < bi.width;++j)
            {
                const int offset = (i * bi.width + j) * bi.channels;
                data[offset] = r;
                if (bi.channels > 1) data[offset + 1] = g;
                if (bi.channels > 2) data[offset + 2] = b;
                if (bi.channels > 3) data[offset + 3] = a;
            }
    }

    void SetColor(uint8_t* data, BufferInfo& buffer_info, int w, int h)
    {
        if (!data || w < 0 || w >= buffer_info.width || h < 0 || h >= buffer_info.height) 
            return;

        const int offset = (h * buffer_info.width + w) * buffer_info.channels;
        data[offset] = 255;
        if (buffer_info.channels > 1) data[offset + 1] = 0;
        if (buffer_info.channels > 2) data[offset + 2] = 0;
        if (buffer_info.channels > 3) data[offset + 3] = 255;
    }

    std::uint8_t* GetBuffer(size_t buffer_object) override
    {
        return datas[buffer_object];
    };
private:
    std::vector<std::uint8_t*> datas;
    std::vector<BufferInfo> bufferInfos;
};
}
#endif