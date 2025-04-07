#ifndef CORE_HAL_WINDOW_IWINDOW_H
#define CORE_HAL_WINDOW_IWINDOW_H

#include <stdint.h>
#include "Core/Macro/Macro.h"
#include "Platform/File/Include/CFile.h"
#include "WindowEnums.h"

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

    virtual void AddKeyDownCallback(KeyDownCallback callback) = 0;
    virtual void AddKeyUpCallback(KeyUpCallback callback) = 0;
    virtual void AddCharInputCallback(CharInputCallback callback) = 0;
    virtual void AddMouseButtonDownCallback(MouseButtonDownCallback callback) = 0;
    virtual void AddMouseButtonUpCallback(MouseButtonUpCallback callback) = 0;
    virtual void AddMouseMoveCallback(MouseMoveCallback callback) = 0;
    virtual void AddMouseScrollCallback(MouseScrollCallback callback) = 0;
    virtual void AddMouseEnterCallback(MouseEnterCallback callback) = 0;
    virtual void AddWindowFocusCallback(WindowFocusCallback callback) = 0;

    virtual void ClearKeyDownCallbacks() = 0;
    virtual void ClearKeyUpCallbacks() = 0;
    virtual void ClearCharInputCallbacks() = 0;
    virtual void ClearMouseButtonDownCallbacks() = 0;
    virtual void ClearMouseButtonUpCallbacks() = 0;
    virtual void ClearMouseMoveCallbacks() = 0;
    virtual void ClearMouseScrollCallbacks() = 0;
    virtual void ClearMouseEnterCallbacks() = 0;
    virtual void ClearWindowFocusCallbacks() = 0;
    virtual void ClearAllCallbacks() = 0; // 清空所有类型的回调
protected:
    char* title;
    uint32_t width;
    uint32_t height;
};
RT_FORCEINLINE IWindow::~IWindow() = default;
#endif
