#ifndef FUNCTION_RENDERER_SIMPLE_RENDERER_H
#define FUNCTION_RENDERER_SIMPLE_RENDERER_H

#include "IRenderer.h"
#include <unordered_map>
#include <cstring>
#include <cmath>
namespace ReiToEngine
{
class RTENGINE_API SimpleRenderer : public IRenderer
{
public:
    ~SimpleRenderer()
    {
        int size = frameBuffers.size();
        for (int i =0; i < size; ++i)
        {
            frameBuffers[i].~FrameBuffer();
        }
        frameBuffers.clear();

        size = vertexBuffers.size();
        for (int i =0; i < size; ++i)
        {
            vertexBuffers[i].~VertexBuffer();
        }
        vertexBuffers.clear();

        size = indiceBuffers.size();
        for (int i =0; i < size; ++i)
        {
            indiceBuffers[i].~IndiceBuffer();
        }
        indiceBuffers.clear();

        size = shaderRenderUnits.size();
        for (int i =0; i < size; ++i)
        {
            shaderRenderUnits[i].~ShaderRenderUnit();
        }
        shaderRenderUnits.clear();

        size = textureRenderUnits.size();
        for (int i =0; i < size; ++i)
        {
            textureRenderUnits[i].~TextureRenderUnit();
        }
        textureRenderUnits.clear();

        size = matrixesRenderUnits.size();
        for (int i =0; i < size; ++i)
        {
            matrixesRenderUnits[i].~MatrixRenderUnit();
        }
        matrixesRenderUnits.clear();

        size = materialRenderUnits.size();
        for (int i =0; i < size; ++i)
        {
            materialRenderUnits[i].~MaterialRenderUnit();
        }
        materialRenderUnits.clear();

        size = lightingRenderUnits.size();
        for (int i =0; i < size; ++i)
        {
            lightingRenderUnits[i].~LightingRenderUnit();
        }
        lightingRenderUnits.clear();
    }

    size_t CreateSceneInfo() override
    {
        sceneInfos.emplace_back();
        return sceneInfos.size() - 1;
    }
    size_t CreateFrameBuffer(uint32_t h, uint32_t w, uint8_t channels) override
    {
        frameBuffers.emplace_back(h, w, channels);
        return frameBuffers.size() - 1;
    }
    size_t CreateVertexBuffer(Vec3d* positions, Vec3d* normals, Vec2d* texCoords, Vec4d* colors, size_t size) override
    {
        vertexBuffers.emplace_back(positions, normals, texCoords, colors, size);
        return vertexBuffers.size() - 1;
    }
    size_t CreateIndiceBuffer(uint32_t* indices, size_t size) override
    {
        indiceBuffers.emplace_back(indices, size);
        return indiceBuffers.size() - 1;
    }
    size_t CreateShaderRenderUnit(std::string& shadercode) override
    {
        shaderRenderUnits.emplace_back(shadercode);
        return shaderRenderUnits.size() - 1;
    }
    size_t CreateTextureRenderUnit(uint32_t w, uint32_t h, uint32_t channels, uint8_t* data) override
    {
        textureRenderUnits.emplace_back(w, h, channels, data);
        return textureRenderUnits.size() - 1;
    }
    size_t CreateMaterialRenderUnit(Vec3d ambientColor, Vec3d diffuseColor, Vec3d specularColor, float shininess) override
    {
        materialRenderUnits.emplace_back(ambientColor, diffuseColor, specularColor, shininess);
        return materialRenderUnits.size() - 1;
    }
    size_t CreateMatrixRenderUnit() override
    {
        matrixesRenderUnits.emplace_back();
        return matrixesRenderUnits.size() - 1;
    }
    void ClearScene(size_t scene_index) override
    {
        SceneInfo* info = &sceneInfos[scene_index];
        info->targetFrames.clear();
        info->usingIndices.clear();
        info->usingMaterials.clear();
        info->usingMatrixes.clear();
        info->usingShaders.clear();
        info->usingTextures.clear();
        info->usingVertexs.clear();
    }
    void BindFrameBuffer(size_t scene_index, size_t frame_buffer_index) override
    {
        sceneInfos[scene_index].targetFrames.push_back(&frameBuffers[frame_buffer_index]);
    }

    void BindObject(size_t scene_index, size_t vertex_buffer_index, size_t indice_buffer_index, size_t material_index) override
    {
        SceneInfo& info = sceneInfos[scene_index];
        info.usingVertexs.push_back(&vertexBuffers[vertex_buffer_index]);
        info.usingIndices.push_back(&indiceBuffers[indice_buffer_index]);
        info.usingMaterials.push_back(&materialRenderUnits[material_index]);
    }
    void BindShader(size_t scene_index, std::string name, size_t shader_index) override
    {
        sceneInfos[scene_index].usingShaders[name] = &shaderRenderUnits[shader_index];
    }
    void BindMatrix(size_t scene_index, size_t matrix_index) override
    {
        sceneInfos[scene_index].usingMatrixes.push_back(&matrixesRenderUnits[matrix_index]);
    }

    void AppendMatrix(size_t matrix_index, Matrix4x4d&& matrix) override
    {
        Matrix4x4d& target = matrixesRenderUnits[matrix_index].matrixes.emplace_back(matrix);
        matrixesRenderUnits[matrix_index].result *= target;
    }
    void AppendMatrix(size_t matrix_index, Matrix4x4d& matrix) override
    {
        matrixesRenderUnits[matrix_index].matrixes.emplace_back(matrix);
        matrixesRenderUnits[matrix_index].result *= matrix;
    }

    void DrawFrame(size_t frame_index, uint8_t* data, size_t size) override
    {
        FrameBuffer& currentFrame = frameBuffers[frame_index];
        SceneInfo& currentScene = *currentFrame.scene;
        size_t object_count = currentScene.usingVertexs.size();
        Matrix4x4d transform_matrix = currentScene.usingMatrixes[0]->result;

    for (size_t i = 0; i < object_count; ++i)
    {
        IndiceBuffer& indices = *currentScene.usingIndices[i];
        if (indices.isLine)
        {
            for (size_t j = 0; j < indices.size / 2; ++j)
            {
                // 注意：这里需要传入变换矩阵和帧缓冲区的宽度和高度
                DrawLine(currentFrame.buffer, currentFrame.zBuffer,
                    currentScene.usingVertexs[i]->positions[indices.indices[j * 2]],
                    currentScene.usingVertexs[i]->positions[indices.indices[j * 2 + 1]],
                    currentScene.usingVertexs[i]->colors[indices.indices[j * 2]],
                    currentScene.usingVertexs[i]->colors[indices.indices[j * 2 + 1]],
                    transform_matrix, currentFrame.width, currentFrame.height
                );
            }
        }
        else
        {
            for (size_t j = 0; j < indices.size / 3; ++j)
            {
                // 注意：这里需要传入顶点的原始坐标 (Vec3d) 和颜色
                DrawTriangle(frame_index,
                    currentScene.usingVertexs[i]->positions[indices.indices[j * 3]],
                    currentScene.usingVertexs[i]->positions[indices.indices[j * 3 + 1]],
                    currentScene.usingVertexs[i]->positions[indices.indices[j * 3 + 2]],
                    currentScene.usingVertexs[i]->colors[indices.indices[j * 3]],
                    currentScene.usingVertexs[i]->colors[indices.indices[j * 3 + 1]],
                    currentScene.usingVertexs[i]->colors[indices.indices[j * 3 + 2]]
                );
            }
        }
    }
        std::memcpy(data, currentFrame.buffer, size);
    }

    void DrawLine(uint8_t* frame_buffer, double_t* z_buffer, Vec3d v1, Vec3d v2, Vec4d color1, Vec4d color2,
    const Matrix4x4d& transform, uint32_t w, uint32_t h)
    {
        Vec4d transformed_v1 = Vec4d({v1.x ,v1.y, v1.z, 0});
        Vec4d transformed_v2 = Vec4d({v2.x ,v2.y, v2.z, 0});
        transformed_v1 = transform * transformed_v1;
        transformed_v2 = transform * transformed_v2;

        int64_t x0 = transformed_v1.x * w; // 使用 Vec3i 的 x 分量
        int64_t y0 = transformed_v1.y * h; // 使用 Vec3i 的 x 分量
        int64_t x1 = transformed_v2.x; // 使用 Vec3i 的 x 分量
        int64_t y1 = transformed_v2.y; // 使用 Vec3i 的 y 分量

        double z0 = transformed_v1.z;
        double z1 = transformed_v2.z;
        bool steep = false;
        if (std::abs(x0-x1)<std::abs(y0-y1))
        {
            std::swap(x0, y0);
            std::swap(y0, y1);
            std::swap(z0, z1);
            steep = true;
        }

        if (x0>x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
            std::swap(color1, color2);
        }

        int dx = x1-x0;
        int dy = y1-y0;
        int derror2 = std::abs(dy)*2;
        int error2 = 0;
        int y = y0;

        Vec4d color_delta = (color2 - color1) / (dx == 0 ? 1 : dx);
        Vec4d color = color1;
        double z_delta = (z1 - z0) / (dx == 0 ? 1 : dx);
        double z = z0;

        for (int x=x0; x<=x1; x++)
        {
            int offset;

            if (steep)
            {
                offset = x * w + y;
            }
            else
            {
                offset = y * w + x;
            }
            // Z-buffer 测试
            if (z < z_buffer[offset])
            {
                z_buffer[offset] = z; // 更新 Z-buffer

                if (steep)
                {
                    SetColor(frame_buffer, frameBuffers[0], y, x, color);
                }
                else
                {
                    SetColor(frame_buffer, frameBuffers[0], x, y, color);
                }
            }
            error2 += derror2;
            if (error2 > dx)
            {
                y += (y1 > y0 ? 1 : -1);
                error2 -= dx * 2;
            }
            color += color_delta; // 更新颜色
            z += z_delta;      // 更新深度
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
void DrawTriangle(size_t buffer_object, Vec3d v0, Vec3d v1, Vec3d v2, Vec4d color0, Vec4d color1, Vec4d color2)
{
    FrameBuffer& frameBuffer = frameBuffers[buffer_object]; // 使用引用
    uint8_t* data = frameBuffer.buffer;
    double_t* zBuffer = frameBuffer.zBuffer;

    // 计算包围盒
    int minX = std::max(0, (int)std::min({v0.x, v1.x, v2.x}));
    int minY = std::max(0, (int)std::min({v0.y, v1.y, v2.y}));
    int maxX = std::min((int)frameBuffer.width - 1,  (int)std::max({v0.x, v1.x, v2.x}));
    int maxY = std::min((int)frameBuffer.height - 1, (int)std::max({v0.y, v1.y, v2.y}));

    // 计算三角形的边向量
    Vec3d edge0 = v1 - v0;
    Vec3d edge1 = v2 - v1;
    Vec3d edge2 = v0 - v2;

    // 计算面积相关的常量 (优化：只计算一次)
    double area = edge0.cross2D(edge1); // 叉乘，注意这里假设顶点顺序是逆时针, 使用 cross2D
    if (std::abs(area) < 1e-6) return; // 退化三角形，直接返回, 避免浮点数精度问题

    // 遍历包围盒
    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            // 计算重心坐标
            Vec3d p = {static_cast<double>(x), static_cast<double>(y), 0.0}; // 当前像素点, 使用 double
            Vec3d w0 = p - v0;
            Vec3d w1 = p - v1;
            Vec3d w2 = p - v2;

            // 计算子三角形面积
            double a0 = w1.cross2D(edge1); // 使用 cross2D
            double a1 = w2.cross2D(edge2); // 使用 cross2D
            double a2 = w0.cross2D(edge0); // 使用 cross2D

            // 检查像素是否在三角形内
            if ((a0 >= 0 && a1 >= 0 && a2 >= 0) || (a0 <= 0 && a1 <= 0 && a2 <= 0))
            {
                // 计算重心坐标
                double alpha = a0 / area;
                double beta = a1 / area;
                double gamma = a2 / area;

                // 计算插值后的 z 值
                double z = alpha * v0.z + beta * v1.z + gamma * v2.z;

                // Z-buffer 测试
                int offset = (y * frameBuffer.width + x); // zBuffer 偏移
                if (z < zBuffer[offset]) //z值越小，离观察者越近
                {
                    // 插值颜色
                    Vec4d interpolated_color = color0 * alpha + color1 * beta + color2 * gamma;

                    // 写入像素
                    SetColor(data, frameBuffer, x, y, interpolated_color);
                    zBuffer[offset] = z; // 更新 Z-buffer
                }
            }
        }
    }
}

    void SetColor(uint8_t* data, FrameBuffer& buffer_info, int w, int h, Vec4d color)
    {
        if (!data || w < 0 || w >= buffer_info.width || h < 0 || h >= buffer_info.height)
            return;

        const int offset = (h * buffer_info.width + w) * buffer_info.channels;
        data[offset] = static_cast<uint8_t>(color.x * 255); // 假设颜色值范围是 [0, 1]
        if (buffer_info.channels > 1) data[offset + 1] = static_cast<uint8_t>(color.y * 255);
        if (buffer_info.channels > 2) data[offset + 2] = static_cast<uint8_t>(color.z * 255);
        if (buffer_info.channels > 3) data[offset + 3] = static_cast<uint8_t>(color.w * 255); // Alpha 通道
    }
    void GetSceneFrameCopy(size_t, uint8_t*, size_t) const override
    {}

private:
    std::vector<SceneInfo> sceneInfos;

    std::vector<FrameBuffer> frameBuffers;
    std::vector<VertexBuffer> vertexBuffers;
    std::vector<IndiceBuffer> indiceBuffers;

    std::vector<ShaderRenderUnit> shaderRenderUnits;
    std::vector<TextureRenderUnit> textureRenderUnits;
    std::vector<MatrixRenderUnit> matrixesRenderUnits;
    std::vector<MaterialRenderUnit> materialRenderUnits;
    std::vector<LightingRenderUnit> lightingRenderUnits;

    bool zBufferEnableState;
    bool zBuffer64Bit;
};
}
#endif
