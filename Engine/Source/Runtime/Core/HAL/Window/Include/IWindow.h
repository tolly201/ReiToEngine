#ifndef CORE_HAL_WINDOW_IWINDOW_H
#define CORE_HAL_WINDOW_IWINDOW_H

#include <stdint.h>
#include "Core/HAL/Macro/Macro.h"
#include "Core/Platform/File/CFile.h"
class RTENGINE_API IWindow
{
public:
    virtual ~IWindow() = 0;
    virtual IWindow* Create(const char* title, uint32_t width, uint32_t height) = 0;
    virtual void SetTitle(const char* title) = 0;
    virtual char* GetTitle() const = 0;

    virtual void SetSize(uint32_t width, uint32_t height) = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void ShowWindow() = 0;
    virtual void HideWindow() = 0;
    virtual void CloseWindow() = 0;

    virtual void ProcessEvents() = 0; // 处理窗口事件

    virtual void Update(const void* buffer, uint32_t width, uint32_t height) = 0; // 更新显示 buffer

protected:
    char* title;
    uint32_t width;
    uint32_t height;
};
RT_FORCEINLINE IWindow::~IWindow() = default;
#endif
