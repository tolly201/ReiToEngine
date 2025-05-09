#include "../../Include/Windows/WindowsWindow.h"

#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Handle window messages here
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

WindowsWindow::WindowsWindow()
    : title(nullptr), width(800), height(600), hwnd(nullptr)
{
}

IWindow* WindowsWindow::Create(const char* title, uint32_t width, uint32_t height)
{
    printf("Creating Windows window...\n");
    i32 posx = 100;
    i32 posy = 100;
    // Create window
    i32 client_x = posx;
    i32 client_y = posy;
    u32 client_width = width;
    u32 client_height = height;

    i32 window_x = posx;
    i32 window_y = posy;
    i32 window_width = client_width;
    i32 window_height = client_height;

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
printf("Window size: %d x %d\n", window_width, window_height);
    if (title) {
        delete[] this->title;
        this->title = new char[strlen(title) + 1];
        memcpy(this->title, title, strlen(title) + 1);
    } else {
        delete[] this->title;
        this->title = new char[256];
        memcpy(this->title, "Kohi Game Engine Window", 22);
    }

    this->width = client_width;
    this->height = client_height;
    f32 device_pixel_ratio = 1.0f;
printf("Device pixel ratio: %f\n", device_pixel_ratio);
    WCHAR wtitle[256];
    int len = MultiByteToWideChar(CP_UTF8, 0, this->title, -1, wtitle, 256);
    if (!len) {
    }

    WNDCLASSEXW wc = {0};
    if (GetClassInfoExW(GetModuleHandleW(0), L"kohi_window_class", &wc)) {
        printf("Window class 'kohi_window_class' is already registered.\n");
    } else {
        // 如果未注册，则注册窗口类
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = DefWindowProcW; // 或自定义窗口过程
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = GetModuleHandleW(0);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = L"kohi_window_class";
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassExW(&wc)) {
            DWORD last_error = GetLastError();
            printf("Failed to register window class. Error: %lu\n", last_error);
            return nullptr;
        }
    }

    printf("Window title: %ls\n", wtitle);
    hwnd = CreateWindowExW(
        window_ex_style, L"kohi_window_class", wtitle,
        window_style, window_x, window_y, window_width, window_height,
        0, 0, GetModuleHandleW(0), 0);
    printf("Window handle: %p\n", hwnd);
    if (hwnd == 0) {
        printf("Window creation failed!\n");

        DWORD last_error = GetLastError();
        LPWSTR wmessage_buf = 0;

        // Ask Win32 to give us the string version of that message ID.
        // The parameters we pass in, tell Win32 to create the buffer that holds the message for us
        // (because we don't yet know how long the message string will be).
        u64 size =
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, last_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&wmessage_buf, 0, NULL);
        printf("Error message size: %d\n", size);

        if (size) {
            // 将宽字符错误信息转换为多字节字符
            int mb_len = WideCharToMultiByte(CP_UTF8, 0, wmessage_buf, -1, NULL, 0, NULL, NULL);
            char* message_buf = new char[mb_len];
            WideCharToMultiByte(CP_UTF8, 0, wmessage_buf, -1, message_buf, mb_len, NULL, NULL);

            MessageBoxW(NULL, wmessage_buf, L"Error!", MB_ICONEXCLAMATION | MB_OK);
            RTFATAL(message_buf);
            int a;
            std::cin >> a;

            delete[] message_buf;
            LocalFree(wmessage_buf);
        }
        else {
            MessageBoxW(NULL, L"Window creation failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
            RTFATAL("Window creation failed!");
            int a;
            std::cin >> a;
        }
        return nullptr;
    }
    else {
        printf("Window created successfully!\n");
    }

    // platform_window_show(window);

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
#endif
