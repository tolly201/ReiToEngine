#ifndef FUNCTION_RENDERER_IWINDOWSMANAGER_H
#define FUNCTION_RENDERER_IWINDOWSMANAGER_H

#include "Core/HAL/Macro/Macro.h"
#include <cstddef>
#include <vector>
namespace ReiToEngine
{
class IRenderer
{
public:
    virtual size_t CreateBuffer() = 0;
    virtual size_t CreateBuffer(uint32_t h, uint32_t w, uint8_t channels) = 0;
    virtual void DrawLine(size_t buffer_object, int x0, int y0, int x1, int y1) = 0;
    virtual std::uint8_t* GetBuffer(size_t buffer_object) = 0;
};

class SimpleRenderer : public IRenderer
{
public:
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
    size_t CreateBuffer(uint32_t h, uint32_t w, uint8_t channels) override
    {
        uint8_t* data = new uint8_t[h * w * channels];
        datas.push_back(data);
    };
    void DrawLine(size_t buffer_object, int x0, int y0, int x1, int y1) override
    {
        uint8_t* data = datas[buffer_object];
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
            SetColor(data, y, x);
        } else {
            SetColor(data, x, y);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
    };

    void SetColor(uint8_t* data, int w, int h)
    {

    }

    std::uint8_t* GetBuffer(size_t buffer_object) override
    {
        return datas[buffer_object];
    };
private:
    std::vector<std::uint8_t*> datas;
};
}
#endif
