#ifndef CORE_HAL_WINDOW_WAYLANDWINDOW_H
#define CORE_HAL_WINDOW_WAYLANDWINDOW_H

#include "../../IWindow.h"
#ifdef RT_SYSTEM_LINUX
#include <wayland-client.h>
#include <wayland-client-protocol.h>

#include "L20_Platform/L35_Input/WaylandInputMonitor.h"

class WaylandWindow : public IWindow {
  public:
    WaylandWindow();
    ~WaylandWindow();

    IWindow*      Create(const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y) override;
    virtual void  SetTitle(const char* title) override;
    virtual char* GetTitle() const override;

    virtual void SetSize(u32 width, u32 height) override;
    virtual u32  GetWidth() const override;
    virtual u32  GetHeight() const override;

    virtual void          ShowWindow() override;
    virtual void          HideWindow() override;
    virtual void          CloseWindow() override;
    void                  SetInputMonitor(PlatformInputMonitor* monitor) override;
    PlatformInputMonitor* GetInputMonitor() const;

    virtual void ProcessEvents() override;
    virtual void Update(const u8* buffer, u32 width, u32 height) override;

  private:
    // Wayland全局对象
    struct wl_display*    m_display;
    struct wl_registry*   m_registry;
    struct wl_compositor* m_compositor;
    struct wl_shell*      m_shell;
    struct wl_seat*       m_seat;
    struct wl_shm*        m_shm;

    // 窗口相关
    struct wl_surface*       m_surface;
    struct wl_shell_surface* m_shell_surface;
    struct wl_egl_window*    m_egl_window;
    struct wl_keyboard*      m_keyboard;
    struct wl_pointer*       m_pointer;
    struct xkb_context*      m_xkb_context;
    struct xkb_keymap*       m_xkb_keymap;
    struct xkb_state*        m_xkb_state;
    struct wl_buffer*        m_buffer;
    PlatformInputMonitor*    inputMonitor;

    static void registryGlobalCallback(void* data, struct wl_registry* registry, uint32_t id, const char* interface,
                                       uint32_t version);
    static void registryGlobalRemoveCallback(void* data, struct wl_registry* registry, uint32_t name);
    // Proper listener declarations will be defined in the .cpp
    static const wl_registry_listener s_registry_listener;
    static const wl_shell_surface_listener s_shell_surface_listener;
};
#endif
#endif
