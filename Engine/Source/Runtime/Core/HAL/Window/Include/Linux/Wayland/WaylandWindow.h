#ifndef CORE_HAL_WINDOW_WAYLANDWINDOW_H
#define CORE_HAL_WINDOW_WAYLANDWINDOW_H

#include "../../IWindow.h"
#ifdef RT_SYSTEM_LINUX
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include <EGL/egl.h>
#include <wayland-egl.h>
class RTENGINE_API IWindow
class WayLandWindow : public IWindow
{
public:
    WayLandWindow();
    ~WayLandWindow();

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
    // Wayland全局对象
    struct wl_display* m_display;
    struct wl_registry* m_registry;
    struct wl_compositor* m_compositor;
    struct wl_shell* m_shell;
    struct wl_seat* m_seat;

    // 窗口相关
    struct wl_surface* m_surface;
    struct wl_shell_surface* m_shell_surface;
    struct wl_egl_window* m_egl_window;

    // 输入相关
    struct wl_keyboard* m_keyboard;
    struct wl_pointer* m_pointer;
    struct xkb_context* m_xkb_context;
    struct xkb_keymap* m_xkb_keymap;
    struct xkb_state* m_xkb_state;

    // 窗口属性
    int m_width;
    int m_height;
    bool m_running;
};
#endif
#endif
