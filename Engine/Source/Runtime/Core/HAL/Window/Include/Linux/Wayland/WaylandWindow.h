#ifndef PLATFORM_HAL_WINDOW_LINUX_WAYLAND_WINDOW_H
#define PLATFORM_HAL_WINDOW_LINUX_WAYLAND_WINDOW_H
#include "Core/MinimalCore.h"
#ifdef RT_SYSTEM_LINUX
#include "../../IWindow.h"
#include <wayland-client.h>
#include <wayland-egl.h>
#include <cstring>
#include <memory>

class WaylandWindow : public IWindow
{
public:
    WaylandWindow();
    virtual ~WaylandWindow();

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
    virtual void Update(const uint8_t* buffer, uint32_t width, uint32_t height) override;

private:
    struct wl_display* display;
    struct wl_registry* registry;
    struct wl_compositor* compositor;
    struct wl_surface* surface;
    struct wl_shell* shell;
    struct wl_shell_surface* shell_surface;
    struct wl_shm* shm;
    struct wl_buffer* buffer;
    bool isVisible;

    // 内部辅助函数
    bool InitializeWayland();
    void DestroyWayland();
    void CreateBuffer(uint32_t width, uint32_t height);
    void DestroyBuffer();
};

#endif
#endif
