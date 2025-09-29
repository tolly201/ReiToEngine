#ifndef PLATFORM_RHI_CPU_SOFT_RENDERER_H
#define PLATFORM_RHI_CPU_SOFT_RENDERER_H
#include <cmath>
#include <cstring>
#include <unordered_map>

#include "../IRenderer.h"

namespace ReiToEngine {
class RTENGINE_API SoftRenderer : public IRenderer {
   public:
    ~SoftRenderer();
    // {
    // int size = frameBuffers.size();
    // for (int i =0; i < size; ++i)
    // {
    //     frameBuffers[i].~FrameBuffer();
    // }
    // frameBuffers.clear();

    // size = vertexBuffers.size();
    // for (int i =0; i < size; ++i)
    // {
    //     vertexBuffers[i].~VertexBuffer();
    // }
    // vertexBuffers.clear();

    // size = indiceBuffers.size();
    // for (int i =0; i < size; ++i)
    // {
    //     indiceBuffers[i].~IndiceBuffer();
    // }
    // indiceBuffers.clear();

    // size = shaderRenderUnits.size();
    // for (int i =0; i < size; ++i)
    // {
    //     shaderRenderUnits[i].~ShaderRenderUnit();
    // }
    // shaderRenderUnits.clear();

    // size = textureRenderUnits.size();
    // for (int i =0; i < size; ++i)
    // {
    //     textureRenderUnits[i].~TextureRenderUnit();
    // }
    // textureRenderUnits.clear();

    // size = matrixesRenderUnits.size();
    // for (int i =0; i < size; ++i)
    // {
    //     matrixesRenderUnits[i].~MatrixRenderUnit();
    // }
    // matrixesRenderUnits.clear();

    // size = materialRenderUnits.size();
    // for (int i =0; i < size; ++i)
    // {
    //     materialRenderUnits[i].~MaterialRenderUnit();
    // }
    // materialRenderUnits.clear();

    // size = lightingRenderUnits.size();
    // for (int i =0; i < size; ++i)
    // {
    //     lightingRenderUnits[i].~LightingRenderUnit();
    // }
    // lightingRenderUnits.clear();
    // }

    size_t CreateSceneInfo() override;
    size_t CreateFrameBuffer(uint32_t h, uint32_t w, uint8_t channels) override;
    size_t CreateVertexBuffer(Vec3d* positions, Vec3d* normals, Vec2d* texCoords, Vec4d* colors, size_t size) override;
    size_t CreateIndiceBuffer(uint32_t* indices, size_t size, bool isLine) override;
    size_t CreateShaderRenderUnit(std::string& shadercode) override;
    size_t CreateTextureRenderUnit(uint32_t w, uint32_t h, uint32_t channels, uint8_t* data) override;
    size_t CreateMaterialRenderUnit(Vec3d ambientColor, Vec3d diffuseColor, Vec3d specularColor,
                                    float shininess) override;
    size_t CreateMatrixRenderUnit() override;
    void ClearScene(size_t scene_index) override;
    void BindFrameBuffer(size_t scene_index, size_t frame_buffer_index) override;
    void BindObject(size_t scene_index, size_t vertex_buffer_index, size_t indice_buffer_index,
                    size_t material_index) override;

    void BindObject(size_t scene_index, size_t vertex_buffer_index, size_t indice_buffer_index) override;

    void BindShader(size_t scene_index, std::string name, size_t shader_index) override;
    void BindMatrix(size_t scene_index, size_t matrix_index) override;
    void AppendMatrix(size_t matrix_index, Matrix4x4d&& matrix) override;
    void AppendMatrix(size_t matrix_index, Matrix4x4d& matrix) override;

    size_t GetFrameSize(size_t frame_index) const;

    void DrawFrame(size_t frame_index, uint8_t*& data, size_t& buffer_size) override;
    void DrawLine(uint8_t* frame_buffer, double_t* z_buffer, Vec3d v1, Vec3d v2, Vec4d color1, Vec4d color2,
                  const Matrix4x4d& transform, uint32_t w, uint32_t h);

    float GetZ(int y, int x, Vec3d v0, Vec3d v1, Vec3d v2);
    void DrawTriangle(uint8_t* data, double* zBuffer, Vec3d v0, Vec3d v1, Vec3d v2, Vec4d color0, Vec4d color1,
                      Vec4d color2, Matrix4x4d transform, uint32_t width, uint32_t height);
    void SetColor(uint8_t* data, FrameBuffer& buffer_info, u32 w, u32 h, Vec4d color);
    void GetSceneFrameCopy(size_t frame_index, uint8_t*& data, size_t& buffer_size) const override;

    void ClearAll()
    {
        frameBuffers.clear();
        vertexBuffers.clear();
        indiceBuffers.clear();
        shaderRenderUnits.clear();
        textureRenderUnits.clear();
        matrixesRenderUnits.clear();
        materialRenderUnits.clear();
        lightingRenderUnits.clear();
    };
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

    [[maybe_unused]]bool zBufferEnableState;
    [[maybe_unused]]bool zBuffer64Bit;
};
}  // namespace ReiToEngine
#endif
