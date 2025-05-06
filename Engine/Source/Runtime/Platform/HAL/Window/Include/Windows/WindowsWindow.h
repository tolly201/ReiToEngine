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

    virtual IWindow* Create(const char* title, uint32_t width, uint32_t height) override;
    virtual void SetTitle(const char* title) override;
    virtual char* GetTitle() const override;

    virtual void SetSize(uint32_t width, uint32_t height) override;
    virtual uint32_t GetWidth() const override;
    virtual uint32_t GetHeight() const override;

    virtual void ShowWindow() override;
    virtual void HideWindow() override;
    virtual void CloseWindow() override;

    virtual void ProcessEvents() override;
    virtual void Update(const u8* buffer, uint32_t width, uint32_t height) override;

private:
    HWND hwnd;
    HINSTANCE hInstance;
    char* title;
    uint32_t width;
    uint32_t height;
};

#endif
#endif
