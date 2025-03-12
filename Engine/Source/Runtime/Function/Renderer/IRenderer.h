#ifndef FUNCTION_RENDERER_IRENDERER_H
#define FUNCTION_RENDERER_IRENDERER_H

#include "Core/HAL/Macro/Macro.h"
#include "Core/Math/IMath.h"
#include <cstddef>
#include <vector>
namespace ReiToEngine
{
// 光照信息
struct LightingRenderUnit
{
    enum class Type {
        Directional,
        Point,
        Spot
    } type;

    Vec3d position;   // 对于点光源和聚光灯
    Vec3d direction;  // 对于方向光和聚光灯
    Vec3d color;
    float intensity;
    float range;      // 对于点光源和聚光灯
    float innerConeAngle; // 对于聚光灯 (弧度)
    float outerConeAngle; // 对于聚光灯 (弧度)

    LightingRenderUnit() : type(Type::Directional), position(0.0f), direction({0.0f, -1.0f, 0.0f}),
                           color(1.0f), intensity(1.0f), range(10.0f),
                           innerConeAngle(0.0f), outerConeAngle(0.0f) {}
};

// 顶点数据
struct VertexBuffer
{
    Vec3d* positions;
    Vec3d* normals;
    Vec2d* texCoords;
    Vec4d* colors;
    size_t size;
    VertexBuffer(){}
    VertexBuffer(Vec3d* _positions,Vec3d* _normals, Vec2d* _texCoords, Vec4d* _colors, size_t _size):
    positions(_positions), normals(_normals), texCoords(_texCoords), colors(_colors), size(_size)
    {}
};

// 索引数据 (通常用 uint32_t, 但也可以用 uint16_t 来节省内存)
// using IndexRenderInfo = uint32_t;  // 不需要单独的结构体，直接用类型别名
// 或者如果你确实需要一个结构体来封装：
struct IndiceBuffer {
    uint32_t* indices;
    size_t size;
    bool isLine = true;

    IndiceBuffer(uint32_t* _indices, size_t _size, bool _isLine):
    indices(_indices), size(_size), isLine(_isLine)
    {}
};

// 着色器信息 (这里只是一个占位符，实际的着色器数据会更复杂)
struct ShaderRenderUnit
{
    std::string shaderCode; // 片段着色器代码
};

// 纹理信息
struct TextureRenderUnit
{
    uint32_t width;
    uint32_t height;
    uint8_t channels;
    uint8_t* data; // 像素数据, 不应该在这里
    TextureRenderUnit() = default;
};

// 材质信息
struct MaterialRenderUnit
{
    Vec3d ambientColor;
    Vec3d diffuseColor;
    Vec3d specularColor;
    float shininess;

    MaterialRenderUnit() : ambientColor({0.2f, 0.2f, 0.2f}), diffuseColor({0.8f, 0.8f, 0.8f}),
                         specularColor({0.5f, 0.5f, 0.5f}), shininess(32.0f) {}
};

// 材质信息
struct MatrixRenderUnit
{
    std::vector<Matrix4x4d> matrixes;
    Matrix4x4d result;

    MatrixRenderUnit()
    {
        matrixes = std::vector<Matrix4x4d>();
        result = Matrix4x4d({1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
    }
};

struct FrameBuffer;
struct SceneInfo {
    bool isActived;
    std::unordered_map<std::string, ShaderRenderUnit*> usingShaders;
    std::vector<FrameBuffer*> targetFrames;

    //one group with same index.
    std::vector<VertexBuffer*> usingVertexs;
    std::vector<IndiceBuffer*> usingIndices;
    std::vector<MaterialRenderUnit*> usingMaterials;

    std::unordered_map<std::string, TextureRenderUnit*> usingTextures;
    std::vector<MatrixRenderUnit*> usingMatrixes;
    SceneInfo() = default;
};

// 帧缓冲区信息
struct FrameBuffer
{
    uint32_t width;
    uint32_t height;
    uint8_t channels; // 通道数 (例如，RGBA 为 4)
    uint8_t* buffer;
    double* zBuffer;
    SceneInfo* scene;

    FrameBuffer() = delete;
    FrameBuffer(uint32_t w, uint32_t h, uint8_t c) : width(w), height(h), channels(c)
    {
        buffer = new uint8_t[w*h*c];
        zBuffer = new double[w*h*c];
        scene = nullptr;
    }
};

class RTENGINE_API IRenderer
{
public:
    virtual size_t CreateSceneInfo() = 0;
    virtual size_t CreateFrameBuffer(uint32_t w, uint32_t h, uint8_t channels) = 0;
    virtual size_t CreateVertexBuffer(Vec3d*, Vec3d*, Vec2d*, Vec4d*, size_t) = 0;
    virtual size_t CreateIndiceBuffer(uint32_t*, size_t, bool) = 0;
    virtual size_t CreateShaderRenderUnit(std::string&) = 0;
    virtual size_t CreateTextureRenderUnit(uint32_t w, uint32_t h, uint32_t channels, uint8_t*) = 0;
    virtual size_t CreateMaterialRenderUnit(Vec3d, Vec3d, Vec3d, float) = 0;
    virtual size_t CreateMatrixRenderUnit() = 0;
    virtual void ClearScene(size_t scene_index) = 0;
    virtual void BindFrameBuffer(size_t scene_index, size_t frame_buffer_index) = 0;

    virtual void BindObject(size_t scene_index, size_t vertex_buffer_index, size_t indice_buffer_index, size_t material_index) = 0;
    virtual void BindObject(size_t scene_index, size_t vertex_buffer_index, size_t indice_buffer_index) = 0;
    virtual void BindShader(size_t scene_index, std::string name, size_t shader_index) = 0;
    virtual void BindMatrix(size_t scene_index, size_t matrix_index) = 0;
    virtual void AppendMatrix(size_t matrix_index, Matrix4x4d&& matrix) = 0;
    virtual void AppendMatrix(size_t matrix_index, Matrix4x4d& matrix) = 0;
    virtual void DrawFrame(size_t frame_index, uint8_t*& data, size_t& buffer_size) = 0;

    virtual void GetSceneFrameCopy(size_t, uint8_t*& data, size_t& buffer_size) const = 0;
};
}
#endif
