#include "WaylandWindow.h"
#ifdef RT_SYSTEM_LINUX
#include <wayland-client-protocol.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "L20_Platform/Include.h"

WaylandWindow::WaylandWindow()
    : m_display(nullptr), m_registry(nullptr), m_compositor(nullptr), m_shell(nullptr),
      m_shm(nullptr), m_surface(nullptr), m_shell_surface(nullptr), m_egl_window(nullptr), m_buffer(nullptr),
      inputMonitor(nullptr),
{
}

WaylandWindow::~WaylandWindow()
{
    CloseWindow();
}

IWindow* WaylandWindow::Create(const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y)
{

    title = title;
    width = width;
    height = height;
    position_x = pos_x;
    position_y = pos_y;


    // 连接到Wayland显示服务器
    m_display = wl_display_connect(nullptr);
    if (!m_display) {
        RT_LOG_FATAL("无法连接到Wayland显示服务器");
        return nullptr;
    }

    // 获取注册表并添加监听器
    m_registry = wl_display_get_registry(m_display);
    wl_registry_add_listener(m_registry, &registry_listener, this);

    // 处理挂起的事件，确保我们获得所有全局对象
    wl_display_roundtrip(m_display);

    // 验证所需的Wayland接口是否可用
    if (!m_compositor || !m_shell || !m_shm) {
        RT_LOG_FATAL("无法获取必要的Wayland接口");
        CloseWindow();
        return nullptr;
    }

    // 创建表面
    m_surface = wl_compositor_create_surface(m_compositor);
    if (!m_surface) {
        RT_LOG_FATAL("无法创建Wayland表面");
        CloseWindow();
        return nullptr;
    }

    // 创建shell表面
    m_shell_surface = wl_shell_get_shell_surface(m_shell, m_shell_surface);
    if (!shell_surface) {
        RT_LOG_FATAL("无法创建Wayland shell表面");
        CloseWindow();
        return nullptr;
    }

    // 设置shell表面监听器
    wl_shell_surface_add_listener(m_shell_surface, &shell_surface_listener, this);
    wl_shell_surface_set_title(m_shell_surface, title);
    wl_shell_surface_set_toplevel(m_shell_surface);

    RT_LOG_INFO("Wayland窗口创建成功");
    return this;
}

void WaylandWindow::SetTitle(const char* title)
{
    this->title = title;
    if (m_shell_surface) {
        wl_shell_surface_set_title(m_shell_surface, title);
    }
}

char* WaylandWindow::GetTitle() const
{
    // 注意：这不是线程安全的，实际应用中应考虑更安全的实现
    char* title = new char[windowTitle.length() + 1];
    strcpy(title, windowTitle.c_str());
    return title;
}

void WaylandWindow::SetSize(uint32_t width, uint32_t height)
{
    windowWidth = width;
    windowHeight = height;

    // 重新创建buffer或通知客户端尺寸改变
    // 这里可能需要根据实际情况实现更复杂的逻辑
    if (surface && buffer) {
        // 销毁旧buffer
        wl_buffer_destroy(buffer);
        buffer = nullptr;

        // 需要重新创建buffer并更新surface
        // 实际实现取决于如何处理缓冲区
    }
}

uint32_t WaylandWindow::GetWidth() const
{
    return windowWidth;
}

uint32_t WaylandWindow::GetHeight() const
{
    return windowHeight;
}

void WaylandWindow::ShowWindow()
{
    isVisible = true;
    // 在Wayland中，表面可见性通常通过提交决定
    if (surface) {
        wl_surface_commit(surface);
    }
}

void WaylandWindow::HideWindow()
{
    isVisible = false;
    // 在Wayland中，隐藏表面可能需要特殊处理
    // 一种方法是不再提交更新
}

void WaylandWindow::CloseWindow()
{
    if (m_buffer) {
        wl_buffer_destroy(m_buffer);
        m_buffer = nullptr;
    }
    if (m_shell_surface) {
        wl_shell_surface_destroy(m_shell_surface);
        m_shell_surface = nullptr;
    }
    if (m_surface) {
        wl_surface_destroy(m_surface);
        m_surface = nullptr;
    }
    if (m_shm) {
        wl_shm_destroy(m_shm);
        m_shm = nullptr;
    }
    if (m_shell) {
        wl_shell_destroy(m_shell);
        m_shell = nullptr;
    }
    if (m_compositor) {
        wl_compositor_destroy(m_compositor);
        m_compositor = nullptr;
    }
    if (m_registry) {
        wl_registry_destroy(m_registry);
        m_registry = nullptr;
    }
    if (m_display) {
        wl_display_disconnect(m_display);
        m_display = nullptr;
    }
}

void WaylandWindow::ProcessEvents()
{
    if (m_display) {
        // 处理所有待处理的Wayland事件
        wl_display_dispatch_pending(m_display);
        // 刷新请求队列
        wl_display_flush(m_display);
    }
}

void WaylandWindow::Update(const uint8_t* buffer_data, uint32_t width, uint32_t height)
{
    ProcessEvents();

    // if (!surface || !shm || width != windowWidth || height != windowHeight) {
    //     return;
    // }

    // // 创建共享内存文件
    // int fd = memfd_create("buffer", 0);
    // if (fd < 0) {
    //     RT_LOG_ERROR("无法创建共享内存文件");
    //     return;
    // }

    // // 设置文件大小
    // size_t stride = width * 4; // 假设ARGB8888格式
    // size_t size = stride * height;
    // ftruncate(fd, size);

    // // 映射内存
    // void* data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // if (data == MAP_FAILED) {
    //     close(fd);
    //     RT_LOG_ERROR("无法映射共享内存");
    //     return;
    // }

    // // 复制像素数据
    // memcpy(data, buffer_data, size);

    // // 创建Wayland共享内存池
    // struct wl_shm_pool* pool = wl_shm_create_pool(shm, fd, size);

    // // 创建缓冲区
    // if (buffer) {
    //     wl_buffer_destroy(buffer);
    // }

    // buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);

    // // 附加缓冲区到表面
    // wl_surface_attach(surface, buffer, 0, 0);
    // wl_surface_damage(surface, 0, 0, width, height);
    // wl_surface_commit(surface);

    // // 清理资源
    // wl_shm_pool_destroy(pool);
    // munmap(data, size);
    // close(fd);
}

// 静态回调函数实现
void WaylandWindow::registryGlobalCallback(void* data, struct wl_registry* registry, uint32_t id,
                                         const char* interface, uint32_t version)
{
}

void WaylandWindow::registryGlobalRemoveCallback(void* data, struct wl_registry* registry, uint32_t name)
{
}

void WaylandWindow::shellSurfaceCallback(void* data, struct wl_shell_surface* shell_surface, uint32_t mode)
{
}

void WindowsWindow::SetInputMonitor(PlatformInputMonitor* monitor)
{
    inputMonitor = monitor;
}

PlatformInputMonitor* WindowsWindow::GetInputMonitor() const
{
    return inputMonitor;
}

#endif
