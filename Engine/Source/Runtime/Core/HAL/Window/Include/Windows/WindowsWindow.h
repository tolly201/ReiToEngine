#ifndef CORE_HAL_WINDOW_WINDOWS_WINDOW_H
#define CORE_HAL_WINDOW_WINDOWS_WINDOW_H
#include "Core/MinimalCore.h"
#ifdef RT_SYSTEM_WINDOWS

#include "../IWindow.h"
#include <windows.h>

class WindowsWindow : public IWindow
{
public:
    WindowsWindow();
    ~WindowsWindow() = default;

    IWindow* Create(const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y) override;
    virtual void SetTitle(const char* title) override;
    virtual char* GetTitle() const override;

    virtual void SetSize(u32 width, u32 height) override;
    virtual u32 GetWidth() const override;
    virtual u32 GetHeight() const override;

    virtual void ShowWindow() override;
    virtual void HideWindow() override;
    virtual void CloseWindow() override;

    virtual void ProcessEvents() override;
    virtual void Update(const u8* buffer, u32 width, u32 height) override;

private:
    HWND hwnd;
};

#endif
#endif
