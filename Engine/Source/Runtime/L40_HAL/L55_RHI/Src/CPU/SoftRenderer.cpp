#include "../../Include/CPU/SoftRenderer.h"

#include <cmath>
#include <cstring>
#include <unordered_map>
#include "L60_Core/Math/VectorInclude.h"

namespace {

inline void setColor([[maybe_unused]]int x, [[maybe_unused]]int y,[[maybe_unused]] int w,[[maybe_unused]] int h,[[maybe_unused]] uint8_t* data,[[maybe_unused]] const ReiToEngine::Vec4d& color,[[maybe_unused]] uint8_t bpp)
{
    [[maybe_unused]]int index =0;
}

[[maybe_unused]] inline void line(int x0, int y0, int x1, int y1,int w, int h, uint8_t* data, const ReiToEngine::Vec4d& color) {
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

    for (int x=x0; x<=x1; x++) {
        float t = (x-x0)/(float)(x1-x0);
        int y = y0*(1.-t) + y1*t;
        if (steep) {
            setColor(y, x,w,h,data, color, 4);
        } else {
            setColor(x, y, w, h, data, color, 4);
        }
    }
}
}

namespace ReiToEngine {
SoftRenderer::~SoftRenderer() = default;
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

size_t SoftRenderer::CreateSceneInfo() {
    sceneInfos.emplace_back();
    return sceneInfos.size() - 1;
}
size_t SoftRenderer::CreateFrameBuffer(uint32_t h, uint32_t w, uint8_t channels) {
    frameBuffers.emplace_back(h, w, channels);
    return frameBuffers.size() - 1;
}
size_t SoftRenderer::CreateVertexBuffer(Vec3d* positions, Vec3d* normals, Vec2d* texCoords, Vec4d* colors,
                                        size_t size) {
    vertexBuffers.emplace_back(positions, normals, texCoords, colors, size);
    return vertexBuffers.size() - 1;
}
size_t SoftRenderer::CreateIndiceBuffer(uint32_t* indices, size_t size, bool isLine) {
    indiceBuffers.emplace_back(indices, size, isLine);
    return indiceBuffers.size() - 1;
}
size_t SoftRenderer::CreateShaderRenderUnit([[maybe_unused]]std::string& shadercode) {
    // shaderRenderUnits.emplace_back(shadercode);
    shaderRenderUnits.emplace_back();
    return shaderRenderUnits.size() - 1;
}
size_t SoftRenderer::CreateTextureRenderUnit([[maybe_unused]]uint32_t w,[[maybe_unused]] uint32_t h,[[maybe_unused]] uint32_t channels,[[maybe_unused]] uint8_t* data) {
    // textureRenderUnits.emplace_back(w, h, channels, data);
    textureRenderUnits.emplace_back();
    return textureRenderUnits.size() - 1;
}
size_t SoftRenderer::CreateMaterialRenderUnit([[maybe_unused]]Vec3d ambientColor,[[maybe_unused]] Vec3d diffuseColor,[[maybe_unused]] Vec3d specularColor,[[maybe_unused]] float shininess) {
    // materialRenderUnits.emplace_back(ambientColor, diffuseColor, specularColor, shininess);
    materialRenderUnits.emplace_back();
    return materialRenderUnits.size() - 1;
}
size_t SoftRenderer::CreateMatrixRenderUnit() {
    matrixesRenderUnits.emplace_back();
    return matrixesRenderUnits.size() - 1;
}
void SoftRenderer::ClearScene(size_t scene_index) {
    SceneInfo* info = &sceneInfos[scene_index];
    info->targetFrames.clear();
    info->usingIndices.clear();
    info->usingMaterials.clear();
    info->usingMatrixes.clear();
    info->usingShaders.clear();
    info->usingTextures.clear();
    info->usingVertexs.clear();
}
void SoftRenderer::BindFrameBuffer(size_t scene_index, size_t frame_buffer_index) {
    sceneInfos[scene_index].targetFrames.push_back(&frameBuffers[frame_buffer_index]);
    frameBuffers[frame_buffer_index].scene = &sceneInfos[scene_index];
}

void SoftRenderer::BindObject(size_t scene_index, size_t vertex_buffer_index, size_t indice_buffer_index,
                              size_t material_index) {
    SceneInfo& info = sceneInfos[scene_index];
    info.usingVertexs.push_back(&vertexBuffers[vertex_buffer_index]);
    info.usingIndices.push_back(&indiceBuffers[indice_buffer_index]);
    info.usingMaterials.push_back(&materialRenderUnits[material_index]);
}

void SoftRenderer::BindObject(size_t scene_index, size_t vertex_buffer_index, size_t indice_buffer_index) {
    SceneInfo& info = sceneInfos[scene_index];
    info.usingVertexs.push_back(&vertexBuffers[vertex_buffer_index]);
    info.usingIndices.push_back(&indiceBuffers[indice_buffer_index]);
}

void SoftRenderer::BindShader(size_t scene_index, std::string name, size_t shader_index) {
    sceneInfos[scene_index].usingShaders[name] = &shaderRenderUnits[shader_index];
}
void SoftRenderer::BindMatrix(size_t scene_index, size_t matrix_index) {
    sceneInfos[scene_index].usingMatrixes.push_back(&matrixesRenderUnits[matrix_index]);
}

void SoftRenderer::AppendMatrix(size_t matrix_index, Matrix4x4d&& matrix) {
    Matrix4x4d& target = matrixesRenderUnits[matrix_index].matrixes.emplace_back(matrix);
    matrixesRenderUnits[matrix_index].result *= target;
}
void SoftRenderer::AppendMatrix(size_t matrix_index, Matrix4x4d& matrix) {
    matrixesRenderUnits[matrix_index].matrixes.emplace_back(matrix);
    matrixesRenderUnits[matrix_index].result = matrix * matrixesRenderUnits[matrix_index].result;
}

size_t SoftRenderer::GetFrameSize(size_t frame_index) const {
    return frameBuffers[frame_index].height * frameBuffers[frame_index].width * frameBuffers[frame_index].channels;
}

void SoftRenderer::DrawFrame(size_t frame_index, uint8_t*& data, size_t& buffer_size) {
    FrameBuffer& currentFrame = frameBuffers[frame_index];
    SceneInfo& currentScene = *currentFrame.scene;
    size_t object_count = currentScene.usingVertexs.size();
    Matrix4x4d transform_matrix = currentScene.usingMatrixes[0]->result;

    size_t frame_size = currentFrame.height * currentFrame.width * currentFrame.channels;
    for (size_t i = 0; i < frame_size; ++i) {
        currentFrame.buffer[i] = 0;
        currentFrame.zBuffer[i] = std::numeric_limits<double>::max();
    }

    for (size_t i = 0; i < object_count; ++i) {
        IndiceBuffer& indices = *currentScene.usingIndices[i];
        if (indices.isLine) {
            for (size_t j = 0; j < indices.size / 2; ++j) {
                // 注意：这里需要传入变换矩阵和帧缓冲区的宽度和高度
                DrawLine(currentFrame.buffer, currentFrame.zBuffer,
                         currentScene.usingVertexs[i]->positions[indices.indices[j * 2]],
                         currentScene.usingVertexs[i]->positions[indices.indices[j * 2 + 1]],
                         currentScene.usingVertexs[i]->colors[indices.indices[j * 2]],
                         currentScene.usingVertexs[i]->colors[indices.indices[j * 2 + 1]], transform_matrix,
                         currentFrame.width, currentFrame.height);
            }
        } else {
            for (size_t j = 0; j < indices.size / 3; ++j) {
                // 注意：这里需要传入顶点的原始坐标 (Vec3d) 和颜色
                DrawTriangle(currentFrame.buffer, currentFrame.zBuffer,
                             currentScene.usingVertexs[i]->positions[indices.indices[j * 3]],
                             currentScene.usingVertexs[i]->positions[indices.indices[j * 3 + 1]],
                             currentScene.usingVertexs[i]->positions[indices.indices[j * 3 + 2]],
                             currentScene.usingVertexs[i]->colors[indices.indices[j * 3]],
                             currentScene.usingVertexs[i]->colors[indices.indices[j * 3 + 1]],
                             currentScene.usingVertexs[i]->colors[indices.indices[j * 3 + 2]], transform_matrix,
                             currentFrame.width, currentFrame.height);
            }
        }
    }

    buffer_size = GetFrameSize(frame_index);
    data = new uint8_t[buffer_size];
    std::memcpy(data, currentFrame.buffer, buffer_size);
}

void SoftRenderer::DrawLine(uint8_t* frame_buffer, double_t* z_buffer, Vec3d v1, Vec3d v2, Vec4d color1, Vec4d color2,
                            const Matrix4x4d& transform, uint32_t w,[[maybe_unused]] uint32_t h) {
    Vec4d transformed_v1 = Vec4d({v1.x, v1.y, v1.z, 1});
    Vec4d transformed_v2 = Vec4d({v2.x, v2.y, v2.z, 1});
    transformed_v1 = transform * transformed_v1;
    transformed_v2 = transform * transformed_v2;

    int64_t x0 = transformed_v1.x;  // 使用 Vec3i 的 x 分量
    int64_t y0 = transformed_v1.y;  // 使用 Vec3i 的 x 分量
    int64_t x1 = transformed_v2.x;  // 使用 Vec3i 的 x 分量
    int64_t y1 = transformed_v2.y;  // 使用 Vec3i 的 y 分量
    double z0 = transformed_v1.z;
    double z1 = transformed_v2.z;
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(z0, z1);
        // std::swap(color1, color2);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;

    Vec4d color_delta = (color2 - color1) / (dx == 0 ? 1 : dx);
    Vec4d color = color1;
    double z_delta = (z1 - z0) / (dx == 0 ? 1 : dx);
    double z = z0;

    for (int x = x0; x <= x1; x++) {
        int offset;

        if (steep) {
            offset = x * w + y;
        } else {
            offset = y * w + x;
        }

        // Z-buffer 测试
        if ( offset >=0 && offset < 800 *600 *4 && z < z_buffer[offset]) {
            z_buffer[offset] = z;  // 更新 Z-buffer
            if (steep) {
                SetColor(frame_buffer, frameBuffers[0], y, x, color);
            } else {
                SetColor(frame_buffer, frameBuffers[0], x, y, color);
            }
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
        color += color_delta;  // 更新颜色
        z += z_delta;          // 更新深度
    }
}

float SoftRenderer::GetZ(int y, int x, Vec3d v0, Vec3d v1, Vec3d v2) {
    // 计算法向量
    Vec3d l1 = v1 - v0;
    Vec3d l2 = v2 - v0;
    Vec3d n = Vec3d();
    n.x = l1.y * l2.z - l1.z * l2.y;
    n.y = l1.z * l2.x - l1.x * l2.z;
    n.z = l1.x * l2.y - l1.y * l2.x;

    // 计算平面方程的 D
    int D = -(n.x * v0.x + n.y * v0.y + n.z * v0.z);

    // 求解 z
    if (n.z != 0) {
        // 使用浮点数避免整数除法问题
        double z = (-D - n.x * x - n.y * y) / static_cast<double>(n.z);
        // 返回或存储 z 值
        return z;
    } else {
        return v0.z;
    }
}
void SoftRenderer::DrawTriangle(uint8_t* data, double* zBuffer, Vec3d v0, Vec3d v1, Vec3d v2, Vec4d color0,
                                Vec4d color1, Vec4d color2, Matrix4x4d transform, uint32_t width, uint32_t height) {
    Vec4d transformed_v0 = Vec4d({v0.x, v0.y, v0.z, 1});
    Vec4d transformed_v1 = Vec4d({v1.x, v1.y, v1.z, 1});
    Vec4d transformed_v2 = Vec4d({v2.x, v2.y, v2.z, 1});

    transformed_v0 = transform * transformed_v0;
    transformed_v1 = transform * transformed_v1;
    transformed_v2 = transform * transformed_v2;

    transformed_v0 /= transformed_v0.w;
    transformed_v1 /= transformed_v1.w;
    transformed_v2 /= transformed_v2.w;

    transformed_v0.x = (transformed_v0.x + 1.0) * 0.5 * width;
    transformed_v0.y = (1.0 - transformed_v0.y) * 0.5 * height;
    transformed_v1.x = (transformed_v1.x + 1.0) * 0.5 * width;
    transformed_v1.y = (1.0 - transformed_v1.y) * 0.5 * height;
    transformed_v2.x = (transformed_v2.x + 1.0) * 0.5 * width;
    transformed_v2.y = (1.0 - transformed_v2.y) * 0.5 * height;

    // box
    int minX = std::max(0, (int)std::min({transformed_v0.x, transformed_v1.x, transformed_v2.x}));
    int minY = std::max(0, (int)std::min({transformed_v0.y, transformed_v1.y, transformed_v2.y}));
    int maxX = std::min((int)width - 1, (int)std::max({transformed_v0.x, transformed_v1.x, transformed_v2.x}));
    int maxY = std::min((int)height - 1, (int)std::max({transformed_v0.y, transformed_v1.y, transformed_v2.y}));

    // 计算三角形的边向量
    Vec4d _edge0 = transformed_v1 - transformed_v0;
    Vec4d _edge1 = transformed_v2 - transformed_v1;
    Vec4d _edge2 = transformed_v0 - transformed_v2;

    Vec3d edge0 = {_edge0.x, _edge0.y, _edge0.z};
    Vec3d edge1 = {_edge1.x, _edge1.y, _edge1.z};
    Vec3d edge2 = {_edge2.x, _edge2.y, _edge2.z};

    // 计算面积相关的常量 (优化：只计算一次)
    double area = std::abs(edge0.cross2D(edge1));  // 叉乘，注意这里假设顶点顺序是逆时针, 使用 cross2D

    if (std::abs(area) < 1e-6) return;  // 退化三角形，直接返回, 避免浮点数精度问题

    // 遍历包围盒
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            // 计算重心坐标
            Vec3d p = {static_cast<double>(x), static_cast<double>(y), 0.0};  // 当前像素点, 使用 double
            Vec3d w0 = p - Vec3d({transformed_v0.x, transformed_v0.y, 0});
            Vec3d w1 = p - Vec3d({transformed_v1.x, transformed_v1.y, 0});
            Vec3d w2 = p - Vec3d({transformed_v2.x, transformed_v2.y, 0});

            // 计算子三角形面积
            double a0 = w1.cross2D(edge1);  // 使用 cross2D
            double a1 = w2.cross2D(edge2);  // 使用 cross2D
            double a2 = w0.cross2D(edge0);  // 使用 cross2D

            // 检查像素是否在三角形内
            if ((a0 >= 0 && a1 >= 0 && a2 >= 0) || (a0 <= 0 && a1 <= 0 && a2 <= 0)) {
                // 统一符号，确保 alpha, beta, gamma 为正
                double sign = (a0 >= 0 && a1 >= 0 && a2 >= 0) ? 1.0 : -1.0;
                double abs_a0 = sign * a0;
                double abs_a1 = sign * a1;
                double abs_a2 = sign * a2;

                // 计算重心坐标
                double alpha = abs_a0 / area;
                double beta = abs_a1 / area;
                double gamma = abs_a2 / area;

                // 计算插值后的 z 值
                [[maybe_unused]]double z = 1.0 / (alpha / transformed_v0.w + beta / transformed_v1.w + gamma / transformed_v2.w);
                double interpolated_z = alpha * transformed_v0.z + beta * transformed_v1.z + gamma * transformed_v2.z;

                int offset = y * width + x;
                if (interpolated_z < zBuffer[offset])  // z值越小，离观察者越近
                {
                    // 插值颜色
                    Vec4d interpolated_color = color0 * alpha + color1 * beta + color2 * gamma;

                    // 写入像素
                    SetColor(data, frameBuffers[0], x, y, interpolated_color);
                    zBuffer[offset] = interpolated_z;  // 更新 Z-buffer
                }
            }
        }
    }
}

void SoftRenderer::SetColor(uint8_t* data, FrameBuffer& buffer_info, u32 w, u32 h, Vec4d color) {
    if (!data || w < 0 || w >= buffer_info.width || h < 0 || h >= buffer_info.height) return;

    const int offset = (h * buffer_info.width + w) * buffer_info.channels;
    data[offset] = color.x;  // 假设颜色值范围是 [0, 1]
    if (buffer_info.channels > 1) data[offset + 1] = color.y;
    if (buffer_info.channels > 2) data[offset + 2] = color.z;
    if (buffer_info.channels > 3) data[offset + 3] = color.w;
}
void SoftRenderer::GetSceneFrameCopy(size_t frame_index, uint8_t*& data, size_t& buffer_size) const {
    buffer_size = GetFrameSize(frame_index);
    delete[] data;
    data = new uint8_t[buffer_size];
    std::memcpy(data, frameBuffers[frame_index].buffer, buffer_size);
}
}  // namespace ReiToEngine
