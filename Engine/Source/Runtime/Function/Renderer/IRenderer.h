#ifndef FUNCTION_RENDERER_IRENDERER_H
#define FUNCTION_RENDERER_IRENDERER_H

#include "Core/HAL/Macro/Macro.h"
#include "Core/Math/IMath.h"
#include <cstddef>
#include <vector>
namespace ReiToEngine
{
class RTENGINE_API IRenderer
{
public:
    virtual size_t CreateBuffer() = 0;
    virtual size_t CreateBuffer(uint32_t h, uint32_t w, uint8_t channels) = 0;
    virtual void DrawLine(size_t buffer_object, Vec3i v1, Vec3i v2) = 0;
    virtual void DrawTriangle(size_t buffer_object, Vec3i v1, Vec3i v2, Vec3i v3, Vec3i color) = 0;
    virtual std::uint8_t* GetBuffer(size_t buffer_object) = 0;
};

class RTENGINE_API SimpleRenderer : public IRenderer
{
public:
    struct BufferInfo
    {
        uint32_t height;
        uint32_t width;
        uint8_t channels;
    };

    ~SimpleRenderer()
    {
        int size = datas.size();
        for (int i =0; i < size; ++i)
        {
            delete[] datas[i];
        }
        datas.clear();

        size = zBuffers.size();

        for (int i =0; i < size; ++i)
        {
            delete[] zBuffers[i];
        }
        zBuffers.clear();
    }
    size_t CreateBuffer() override
    {
        uint8_t* data = new uint8_t[1];
        size_t index = datas.size();
        datas.push_back(data);
        CreateZBuffer(1);
        return index;
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
        CreateZBuffer(h*w*channels);
        return index;
    };

    size_t CreateZBuffer(uint32_t length)
    {
        double* buffer = new double[length];
        for (uint32_t i = 0; i < length; ++i)
        {
            buffer[i] = std::numeric_limits<double>::max();
        }
        zBuffers.emplace_back(buffer);
        return length;
    }

    void DrawLine(size_t buffer_object, Vec3i v1, Vec3i v2) override
    {
        uint8_t* data = datas[buffer_object];
        BufferInfo bi = bufferInfos[buffer_object];
        int x0 = v1.x; // 使用 Vec3i 的 x 分量
        int y0 = v1.y; // 使用 Vec3i 的 y 分量
        int x1 = v2.x; // 使用 Vec3i 的 x 分量
        int y1 = v2.y; // 使用 Vec3i 的 y 分量
        bool steep = false;
        if (std::abs(x0-x1)<std::abs(y0-y1))
        {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }

        if (x0>x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        int dx = x1-x0;
        int dy = y1-y0;
        int derror2 = std::abs(dy)*2;
        int error2 = 0;
        int y = y0;
        for (int x=x0; x<=x1; x++)
        {
            if (steep)
            {
                SetColor(data, bi, y, x);
            }
            else
            {
                SetColor(data, bi, x, y);
            }
            error2 += derror2;
            if (error2 > dx) {
                y += (y1>y0?1:-1);
                error2 -= dx*2;
            }
        }
    }

    float GetZ(int y, int x, Vec3i v0, Vec3i v1, Vec3i v2)
    {
        // 计算法向量
        Vec3i l1 = v1 - v0;
        Vec3i l2 = v2 - v0;
        Vec3i n = Vec3i();
        n.x = l1.y*l2.z - l1.z * l2.y;
        n.y = l1.z*l2.x - l1.x * l2.z;
        n.z = l1.x * l2.y - l1.y * l2.x;

        // 计算平面方程的 D
        int D = -(n.x * v0.x + n.y * v0.y + n.z * v0.z);

        // 求解 z
        if (n.z != 0)
        {
            // 使用浮点数避免整数除法问题
            float z = (-D - n.x * x - n.y * y) / static_cast<float>(n.z);
            // 返回或存储 z 值
            return z;
        }
        else
        {
            return v0.z;
        }
    }

    void DrawTriangle(size_t buffer_object, Vec3i v0, Vec3i v1, Vec3i v2, Vec3i color) override
    {
        uint8_t* data = datas[buffer_object];
        BufferInfo bi = bufferInfos[buffer_object];
        double* zBuffer = zBuffers[buffer_object];
        Vec3i edge_A, edge_B, edge_C;
        edge_A.x = v1.y - v0.y; // 使用 Vec3i 的 y 分量
        edge_A.y = v1.x - v0.x; // 使用 Vec3i 的 x 分量
        edge_B.x = v2.y - v1.y; // 使用 Vec3i 的 y 分量
        edge_B.y = v2.x - v1.x; // 使用 Vec3i 的 x 分量
        edge_C.x = v0.y - v2.y; // 使用 Vec3i 的 y 分量
        edge_C.y = v0.x - v2.x; // 使用 Vec3i 的 x 分量

        Vec3i box_min, box_max;
        box_min.x = std::max(std::min(std::min(v0.x, v1.x), v2.x), 0); // 使用 Vec3i 的 x 分量
        box_min.y = std::max(std::min(std::min(v0.y, v1.y), v2.y), 0); // 使用 Vec3i 的 y 分量
        box_max.x = std::min(static_cast<uint32_t>(std::max(std::max(v0.x, v1.x), v2.x)), bi.width - 1); // 使用 Vec3i 的 x 分量
        box_max.y = std::min(static_cast<uint32_t>(std::max(std::max(v0.y, v1.y), v2.y)), bi.height - 1); // 使用 Vec3i 的 y 分量

        printf("bi: %d, %d, %d\n", bi.width, bi.height, bi.channels);
        printf("box: %d, %d, %d, %d\n", box_min.x, box_min.y, box_max.x,box_max.y);
        for (int i = box_min.y; i <= box_max.y; ++i)
        for (int j = box_min.x; j <= box_max.x; ++j)
        {
            Vec3i line_A, line_B, line_C;
            line_A.x = i - v0.y; // 使用 Vec3i 的 y 分量
            line_A.y = j - v0.x; // 使用 Vec3i 的 x 分量
            line_B.x = i - v1.y; // 使用 Vec3i 的 y 分量
            line_B.y = j - v1.x; // 使用 Vec3i 的 x 分量
            line_C.x = i - v2.y; // 使用 Vec3i 的 y 分量
            line_C.y = j - v2.x; // 使用 Vec3i 的 x 分量

            int re1, re2, re3;
            re1 = edge_A.x * line_A.y - edge_A.y * line_A.x;
            re2 = edge_B.x * line_B.y - edge_B.y * line_B.x;
            re3 = edge_C.x * line_C.y - edge_C.y * line_C.x;
            printf("point {%d, %d}, result:{%d, %d}, {%d, %d}, {%d, %d}, {%d, %d, %d},\n",
                i,j,line_A.x, line_A.y, line_B.x, line_B.y, line_C.x, line_C.y,
                re1,re2,re3
            );
            if ((re1 >= 0 && re2 >=0 && re3 >=0) || (re1 <= 0 && re2 <=0 && re3 <=0))
            {
                float _z = GetZ(i, j, v0, v1, v2);
                const int offset = (i * bi.width + j) * bi.channels;
                if (_z > zBuffer[offset])
                {
                    printf("z test failed: %.3f, %.3f\n", _z, zBuffer[offset]);
                }
                else
                {
                    printf("z test passed: %.3f, %.3f\n", _z, zBuffer[offset]);
                    zBuffer[offset] = _z;
                    zBuffer[offset + 1] = _z;
                    zBuffer[offset + 2] = _z;
                    zBuffer[offset + 3] = _z;
                    data[offset] = color.x;
                    if (bi.channels > 1) data[offset + 1] = color.y;
                    if (bi.channels > 2) data[offset + 2] = color.z;
                    if (bi.channels > 3) data[offset + 3] = 255;
                }
            }
        }
    }
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
    std::vector<double*> zBuffers;
    bool zBufferEnableState;
    bool zBuffer64Bit;

};
}
#endif
