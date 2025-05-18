#ifndef CORE_HAL_WINDOW_IWINDOWVIEW_H
#define CORE_HAL_WINDOW_IWINDOWVIEW_H

#include <stdint.h>
#include "Core/Macro/Macro.h"
#include "Platform/File/Include/CFile.h"
#include "IWindow.h"
class RTENGINE_API IWindowView
{
public:
    IWindowView() = default; // 构造函数
    virtual ~IWindowView() = 0; // 虚析构函数

    // 设置和获取 Buffer 相关信息
    virtual void SetBuffer(uint8_t* buffer) = 0;
    virtual uint8_t* GetBuffer() const = 0;

    virtual void SetWidth(uint32_t width) = 0;
    virtual uint32_t GetWidth() const = 0;

    virtual void SetHeight(uint32_t height) = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetChannel(uint8_t channel) = 0;
    virtual uint8_t GetChannel() const = 0;
    IWindow* ownerWindow;
protected:
    uint8_t* buffer;
    uint32_t width;
    uint32_t height;
    uint8_t channel;
};

RT_FORCEINLINE IWindowView::~IWindowView() = default;
#endif
