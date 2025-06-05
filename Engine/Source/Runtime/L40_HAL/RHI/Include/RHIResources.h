// Render Hardware Interface Resources Header
#pragma once

#include "CoreMinimal.h"
#include "RHIDefinitions.h"

// 纹理资源
class RHI_API FRHITexture : public FRHIResource
{
public:
    // 获取纹理宽度
    virtual uint32 GetWidth() const = 0;

    // 获取纹理高度
    virtual uint32 GetHeight() const = 0;
};

// 缓冲区资源
class RHI_API FRHIBuffer : public FRHIResource
{
public:
    // 获取缓冲区大小
    virtual uint32 GetSize() const = 0;
};

// 渲染管线状态
class RHI_API FRHIPipelineState
{
public:
    // 设置着色器
    virtual void SetShader(FRHIResource* Shader) = 0;
};
