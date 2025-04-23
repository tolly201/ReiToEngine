// Render Hardware Interface Definitions Header
#pragma once

// 像素格式定义
enum class EPixelFormat : uint8
{
    PF_Unknown,
    PF_R8G8B8A8,       // 常用的 8 位 RGBA 格式
    PF_DepthStencil,   // 深度/模板缓冲格式
    PF_BC1,            // 压缩格式
    // ... 其他格式
};

// 资源绑定类型
enum class EResourceBindFlags : uint8
{
    None = 0,
    VertexBuffer = 1 << 0,  // 顶点缓冲区
    IndexBuffer = 1 << 1,   // 索引缓冲区
    ShaderResource = 1 << 2 // 着色器资源
};

// 渲染目标类型
enum class ERenderTargetLoadAction : uint8
{
    Load,   // 加载现有内容
    Clear,  // 清空
    DontCare // 不关心
};

enum class ERenderTargetStoreAction : uint8
{
    Store,   // 存储渲染结果
    DontCare // 不关心
};
