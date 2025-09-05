#ifndef PLATFORM_WINDOW_IWINDOW_H
#define PLATFORM_WINDOW_IWINDOW_H
#include "L0_Macro/Include.h"
#include "WindowEnums.h"
#include "L20_Platform/L35_Input/InputSystem.h"
class RTENGINE_API IWindow
{
public:
    virtual ~IWindow() = 0;
    virtual IWindow* Create(const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y) = 0;
    virtual void SetTitle(const char* title) = 0;
    virtual char* GetTitle() const = 0;

    virtual void SetSize(u32 width, u32 height) = 0;
    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0;

    virtual void ShowWindow() = 0;
    virtual void HideWindow() = 0;
    virtual void CloseWindow() = 0;

    virtual void ProcessEvents() = 0;

    virtual void SetInputMonitor(PlatformInputMonitor* monitor) = 0;
    virtual void Update(const u8* buffer, u32 width, u32 height) = 0;

    virtual void* GetNativeWindowHandle() const = 0;
protected:
    char* title;
    u32 width;
    u32 height;
    u32 position_x;
    u32 position_y;
};
RT_FORCEINLINE IWindow::~IWindow() = default;
#endif
