#include "WindowsWindow.h"

#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
#include "L20_Platform/L23_Logger/Logger.h"
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Handle window messages here
    PlatformInputMonitor* monitor = reinterpret_cast<PlatformInputMonitor*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (monitor) {
        MSG msg = { hwnd, uMsg, wParam, lParam };
        monitor->ProcessNativeEvent();
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

WindowsWindow::WindowsWindow()
{
    this->title  = nullptr;
    this->width = 0;
    this->height = 0;
    this->hwnd = nullptr;
    this->position_x = 0;
    this->position_y = 0;
}

IWindow* WindowsWindow::Create(const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y)
{
    RT_LOG_INFO("Creating Windows window...\n");
    // Create window

    i32 client_x = pos_x;
    i32 client_y = pos_y;
    u32 client_width = width;
    u32 client_height = height;

    i32 window_x = client_x;
    i32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    // Obtain the size of the border.
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // In this case, the border rectangle is negative.
    window_x += border_rect.left;
    window_y += border_rect.top;

    // Grow by the size of the OS border.
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;


    RT_LOG_INFO("Window size: %d x %d\n", window_width, window_height);
    if (title) {
        delete[] this->title;
        this->title = new char[strlen(title) + 1];
        memcpy(this->title, title, strlen(title) + 1);
    } else {
        delete[] this->title;
        this->title = new char[256];
        memcpy(this->title, "ReiToEngine Window", 19);
    }

    this->width = client_width;
    this->height = client_height;
    this->position_x = client_x;
    this->position_y = client_y;

    f32 device_pixel_ratio = 1.0f;
RT_LOG_INFO("Device pixel ratio: %f\n", device_pixel_ratio);
    WCHAR wtitle[256];
    int len = MultiByteToWideChar(CP_UTF8, 0, this->title, -1, wtitle, 256);
    if (!len) {
    }

    RT_LOG_INFO("Window title: %ls\n", this->title);
    RT_LOG_INFO("ready to create\n");

    hwnd = CreateWindowExW(
        window_ex_style, L"rt_base_window_class", wtitle,
        window_style, window_x, window_y, window_width, window_height,
        0, 0, GetModuleHandleW(0), 0);

    RT_LOG_INFO("Window handle: %p\n", hwnd);
    if (hwnd == 0) {
        RT_LOG_INFO("Window creation failed!\n");

        MessageBoxW(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);

        RT_LOG_FATAL("Window creation failed!");
    }
    else {
        RT_LOG_INFO("Window created successfully!\n");
    }

    return this;
}

void WindowsWindow::SetTitle(const char* title)
{
    this->title = const_cast<char*>(title);
    std::wstring wideTitle = std::wstring(title, title + strlen(title));
    SetWindowText(hwnd, wideTitle.c_str());
}

char* WindowsWindow::GetTitle() const
{
    return title;
}

void WindowsWindow::SetSize(uint32_t width, uint32_t height)
{
    this->width = width;
    this->height = height;
    SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOMOVE);
}

uint32_t WindowsWindow::GetWidth() const
{
    return width;
}

uint32_t WindowsWindow::GetHeight() const
{
    return height;
}

void WindowsWindow::ShowWindow()
{
    b32 should_activate = 1; // TODO: if the window should not accept input, this should be false.
    i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ::ShowWindow(hwnd, show_window_command_flags);

}

void WindowsWindow::HideWindow()
{
    ::ShowWindow(hwnd, SW_HIDE);
}

void WindowsWindow::CloseWindow()
{
    DestroyWindow(hwnd);
    hwnd = nullptr;
}

void WindowsWindow::ProcessEvents()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WindowsWindow::Update(const u8* buffer, uint32_t width, uint32_t height)
{
    // Update the display buffer if necessary
}

void WindowsWindow::SetInputMonitor(PlatformInputMonitor* monitor)
{
    inputMonitor = monitor;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(inputMonitor));
}

PlatformInputMonitor* WindowsWindow::GetInputMonitor() const
{
    return inputMonitor;
}
#endif
