#ifndef PLATFORM_RHI_IRENDERER_H
#define PLATFORM_RHI_IRENDERER_H

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "L0_Macro/Include.h"
#include "L20_Platform/Include.h"
#include "RenderStructs.h"
namespace ReiToEngine {
class RTENGINE_API IRenderer {
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

    virtual void BindObject(size_t scene_index, size_t vertex_buffer_index, size_t indice_buffer_index,
                            size_t material_index) = 0;
    virtual void BindObject(size_t scene_index, size_t vertex_buffer_index, size_t indice_buffer_index) = 0;
    virtual void BindShader(size_t scene_index, std::string name, size_t shader_index) = 0;
    virtual void BindMatrix(size_t scene_index, size_t matrix_index) = 0;
    virtual void AppendMatrix(size_t matrix_index, Matrix4x4d&& matrix) = 0;
    virtual void AppendMatrix(size_t matrix_index, Matrix4x4d& matrix) = 0;
    virtual void DrawFrame(size_t frame_index, uint8_t*& data, size_t& buffer_size) = 0;

    virtual void GetSceneFrameCopy(size_t, uint8_t*& data, size_t& buffer_size) const = 0;
};
}  // namespace ReiToEngine
#endif
