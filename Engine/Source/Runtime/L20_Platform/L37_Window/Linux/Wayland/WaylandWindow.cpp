#include "WaylandWindow.h"
#ifdef RT_SYSTEM_LINUX
#include <wayland-client-protocol.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "L20_Platform/Include.h"

// // 注册全局对象的回调
// const struct wl_registry_listener registry_listener = {
//     WaylandWindow::registryGlobalCallback,
//     WaylandWindow::registryGlobalRemoveCallback
// };

// // Shell表面配置回调
// const struct wl_shell_surface_listener shell_surface_listener = {
//     WaylandWindow::shellSurfaceCallback
// };

WaylandWindow::WaylandWindow()
    : display(nullptr), registry(nullptr), compositor(nullptr), shell(nullptr),
      surface(nullptr), shell_surface(nullptr), shm(nullptr), buffer(nullptr),
      windowWidth(0), windowHeight(0), isVisible(false)
{
}

WaylandWindow::~WaylandWindow()
{
    CloseWindow();
}

IWindow* WaylandWindow::Create(const char* title, uint32_t width, uint32_t height)
{
    windowTitle = title;
    windowWidth = width;
    windowHeight = height;

    // 连接到Wayland显示服务器
    display = wl_display_connect(nullptr);
    if (!display) {
        RT_LOG_FATAL("无法连接到Wayland显示服务器");
        return nullptr;
    }

    // 获取注册表并添加监听器
    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);

    // 处理挂起的事件，确保我们获得所有全局对象
    wl_display_roundtrip(display);

    // 验证所需的Wayland接口是否可用
    if (!compositor || !shell || !shm) {
        RT_LOG_FATAL("无法获取必要的Wayland接口");
        CloseWindow();
        return nullptr;
    }

    // 创建表面
    surface = wl_compositor_create_surface(compositor);
    if (!surface) {
        RT_LOG_FATAL("无法创建Wayland表面");
        CloseWindow();
        return nullptr;
    }

    // 创建shell表面
    shell_surface = wl_shell_get_shell_surface(shell, surface);
    if (!shell_surface) {
        RT_LOG_FATAL("无法创建Wayland shell表面");
        CloseWindow();
        return nullptr;
    }

    // 设置shell表面监听器
    wl_shell_surface_add_listener(shell_surface, &shell_surface_listener, this);
    wl_shell_surface_set_title(shell_surface, windowTitle.c_str());
    wl_shell_surface_set_toplevel(shell_surface);

    RT_LOG_INFO("Wayland窗口创建成功");
    return this;
}

void WaylandWindow::SetTitle(const char* title)
{
    windowTitle = title;
    if (shell_surface) {
        wl_shell_surface_set_title(shell_surface, windowTitle.c_str());
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
    if (buffer) {
        wl_buffer_destroy(buffer);
        buffer = nullptr;
    }

    if (shell_surface) {
        wl_shell_surface_destroy(shell_surface);
        shell_surface = nullptr;
    }

    if (surface) {
        wl_surface_destroy(surface);
        surface = nullptr;
    }

    if (shm) {
        wl_shm_destroy(shm);
        shm = nullptr;
    }

    if (shell) {
        wl_shell_destroy(shell);
        shell = nullptr;
    }

    if (compositor) {
        wl_compositor_destroy(compositor);
        compositor = nullptr;
    }

    if (registry) {
        wl_registry_destroy(registry);
        registry = nullptr;
    }

    if (display) {
        wl_display_disconnect(display);
        display = nullptr;
    }

    isVisible = false;
}

void WaylandWindow::ProcessEvents()
{
    if (display) {
        // 处理所有待处理的Wayland事件
        wl_display_dispatch_pending(display);
        // 刷新请求队列
        wl_display_flush(display);
    }
}

void WaylandWindow::Update(const uint8_t* buffer_data, uint32_t width, uint32_t height)
{
    if (!surface || !shm || width != windowWidth || height != windowHeight) {
        return;
    }

    // 创建共享内存文件
    int fd = memfd_create("buffer", 0);
    if (fd < 0) {
        RT_LOG_ERROR("无法创建共享内存文件");
        return;
    }

    // 设置文件大小
    size_t stride = width * 4; // 假设ARGB8888格式
    size_t size = stride * height;
    ftruncate(fd, size);

    // 映射内存
    void* data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        RT_LOG_ERROR("无法映射共享内存");
        return;
    }

    // 复制像素数据
    memcpy(data, buffer_data, size);

    // 创建Wayland共享内存池
    struct wl_shm_pool* pool = wl_shm_create_pool(shm, fd, size);

    // 创建缓冲区
    if (buffer) {
        wl_buffer_destroy(buffer);
    }

    buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);

    // 附加缓冲区到表面
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage(surface, 0, 0, width, height);
    wl_surface_commit(surface);

    // 清理资源
    wl_shm_pool_destroy(pool);
    munmap(data, size);
    close(fd);
}

// 静态回调函数实现
void WaylandWindow::registryGlobalCallback(void* data, struct wl_registry* registry, uint32_t id,
                                         const char* interface, uint32_t version)
{
    WaylandWindow* window = static_cast<WaylandWindow*>(data);

    if (strcmp(interface, "wl_compositor") == 0) {
        window->compositor = static_cast<wl_compositor*>(
            wl_registry_bind(registry, id, &wl_compositor_interface, 1));
    } else if (strcmp(interface, "wl_shell") == 0) {
        window->shell = static_cast<wl_shell*>(
            wl_registry_bind(registry, id, &wl_shell_interface, 1));
    } else if (strcmp(interface, "wl_shm") == 0) {
        window->shm = static_cast<wl_shm*>(
            wl_registry_bind(registry, id, &wl_shm_interface, 1));
    }
}

void WaylandWindow::registryGlobalRemoveCallback(void* data, struct wl_registry* registry, uint32_t name)
{
    // 处理全局对象移除
    // 在简单实现中可以不做任何事
}

void WaylandWindow::shellSurfaceCallback(void* data, struct wl_shell_surface* shell_surface, uint32_t mode)
{
    // 处理shell surface配置改变
    // 在简单实现中可以不做任何事
}
#endif
