#ifndef CORE_HAL_WINDOW_WAYLANDWINDOW_H
#define CORE_HAL_WINDOW_WAYLANDWINDOW_H

#include "../../IWindow.h"
#ifdef RT_SYSTEM_LINUX
#include <EGL/egl.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <xkbcommon/xkbcommon.h>

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
    static void shellSurfaceCallback(void* data, struct wl_shell_surface* shell_surface, uint32_t mode);

    const struct wl_registry_listener registry_listener = {WaylandWindow::registryGlobalCallback,
                                                           WaylandWindow::registryGlobalRemoveCallback};

    const struct wl_shell_surface_listener shell_surface_listener = {WaylandWindow::shellSurfaceCallback};

    static void keyboard_keymap_callback(void* data, struct wl_keyboard* keyboard, uint32_t format, int fd,
                                         uint32_t size);
    static void keyboard_enter_callback(void* data, struct wl_keyboard* keyboard, uint32_t serial,
                                        struct wl_surface* surface, struct wl_array* keys);
    static void keyboard_leave_callback(void* data, struct wl_keyboard* keyboard, uint32_t serial,
                                        struct wl_surface* surface);
    static void keyboard_key_callback(void* data, struct wl_keyboard* keyboard, uint32_t serial, uint32_t time,
                                      uint32_t key, uint32_t state);
    static void keyboard_modifiers_callback(void* data, struct wl_keyboard* keyboard, uint32_t serial,
                                            uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked,
                                            uint32_t group);
    static void keyboard_repeat_info_callback(void* data, struct wl_keyboard* keyboard, int rate, int delay);

    const struct wl_keyboard_listener keyboard_listener = {
        WaylandWindow::keyboard_keymap_callback,    WaylandWindow::keyboard_enter_callback,
        WaylandWindow::keyboard_leave_callback,     WaylandWindow::keyboard_key_callback,
        WaylandWindow::keyboard_modifiers_callback, WaylandWindow::keyboard_repeat_info_callback};

    // 鼠标事件监听器
    static void pointer_enter_callback(void* data, struct wl_pointer* pointer, uint32_t serial,
                                       struct wl_surface* surface, wl_fixed_t sx, wl_fixed_t sy);

    static void pointer_leave_callback(void* data, struct wl_pointer* pointer, uint32_t serial,
                                       struct wl_surface* surface);

    static void pointer_motion_callback(void* data, struct wl_pointer* pointer, uint32_t time, wl_fixed_t sx,
                                        wl_fixed_t sy);

    static void pointer_button_callback(void* data, struct wl_pointer* pointer, uint32_t serial, uint32_t time,
                                        uint32_t button, uint32_t state);

    static void pointer_axis_callback(void* data, struct wl_pointer* pointer, uint32_t time, uint32_t axis,
                                      wl_fixed_t value);

    static void pointer_frame_callback(void* data, struct wl_pointer* pointer);

    static void pointer_axis_source_callback(void* data, struct wl_pointer* pointer, uint32_t source);

    static void pointer_axis_stop_callback(void* data, struct wl_pointer* pointer, uint32_t time, uint32_t axis);
    static void pointer_axis_discrete_callback(void* data, struct wl_pointer* pointer, uint32_t axis, int32_t discrete);

    const struct wl_pointer_listener {
        pointer_enter_callback, pointer_leave_callback, pointer_motion_callback, pointer_button_callback,
            pointer_axis_callback, pointer_frame_callback, pointer_axis_source_callback, pointer_axis_stop_callback,
            pointer_axis_discrete_callback
    };
    // 触摸事件监听器
    static void touch_down_callback(void* data, struct wl_touch* touch, uint32_t serial, uint32_t time,
                                    struct wl_surface* surface, int32_t id, wl_fixed_t x, wl_fixed_t y);
    static void touch_up_callback(void* data, struct wl_touch* touch, uint32_t serial, uint32_t time, int32_t id);

    static void touch_motion_callback(void* data, struct wl_touch* touch, uint32_t time, int32_t id, wl_fixed_t x,
                                      wl_fixed_t y);

    static void touch_frame_callback(void* data, struct wl_touch* touch);

    static void touch_cancel_callback(void* data, struct wl_touch* touch);

    static void touch_shape_callback(void* data, struct wl_touch* touch, int32_t id, wl_fixed_t major,
                                     wl_fixed_t minor);

    static void touch_orientation_callback(void* data, struct wl_touch* touch, int32_t id, wl_fixed_t orientation);
    const struct wl_touch_listener {
        touch_down_callback, touch_up_callback, touch_motion_callback, touch_frame_callback, touch_cancel_callback,
            touch_shape_callback, touch_orientation_callback
    };

    // 平板/手写板等扩展设备监听器（以 tablet_tool 为例，需 Wayland 扩展支持）
    static void tablet_tool_type_callback(void* data, struct zwp_tablet_tool_v2* tool, uint32_t type);
    static void tablet_tool_enter_callback(void* data, struct zwp_tablet_tool_v2* tool, uint32_t serial,
                                           struct zwp_tablet_v2* tablet, struct wl_surface* surface);
    static void tablet_tool_leave_callback(void* data, struct zwp_tablet_tool_v2* tool, uint32_t serial,
                                           struct wl_surface* surface);
    static void tablet_tool_motion_callback(void* data, struct zwp_tablet_tool_v2* tool, wl_fixed_t x, wl_fixed_t y);
    static void tablet_tool_pressure_callback(void* data, struct zwp_tablet_tool_v2* tool, uint32_t pressure);
    static void tablet_tool_button_callback(void* data, struct zwp_tablet_tool_v2* tool, uint32_t serial,
                                            uint32_t button, uint32_t state);

    const struct zwp_tablet_tool_v2_listener {
        tablet_tool_type_callback,
            nullptr, // hardware_serial
            nullptr, // hardware_id_wacom
            tablet_tool_enter_callback, tablet_tool_leave_callback, tablet_tool_motion_callback,
            tablet_tool_pressure_callback,
            nullptr, // distance
            nullptr, // tilt
            nullptr, // rotation
            nullptr, // slider
            tablet_tool_button_callback,
            nullptr, // frame
            nullptr, // proximity_in
            nullptr  // proximity_out
    };
};
#endif
#endif
