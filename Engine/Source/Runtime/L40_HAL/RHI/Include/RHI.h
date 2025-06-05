// Render Hardware Interface Core Header
#pragma once

#include "CoreMinimal.h"
#include "RHIDefinitions.h"

// 渲染指令列表
class RHI_API FRHICommandList
{
public:
    // 提交指令列表到 GPU
    void Submit();

    // 设置渲染目标
    void SetRenderTarget(FRHITexture* RenderTarget, FRHITexture* DepthStencilTarget);

    // 绘制调用
    void DrawIndexedPrimitive(FRHIBuffer* IndexBuffer, int32 BaseVertexIndex, uint32 NumPrimitives);
};

// 渲染资源基类
class RHI_API FRHIResource
{
public:
    virtual ~FRHIResource() {}
};

// 动态 RHI 接口
class RHI_API FDynamicRHI
{
public:
    // 初始化 RHI
    static FDynamicRHI* Create();

    // 销毁 RHI
    static void Shutdown();

    // 创建纹理资源
    virtual FRHITexture* CreateTexture(uint32 Width, uint32 Height, EPixelFormat Format) = 0;

    // 创建缓冲区资源
    virtual FRHIBuffer* CreateBuffer(uint32 Size, uint32 Usage) = 0;
};
