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
    this->title = const_cast<char*>(title);
    this->width = width;
    this->height = height;

    hInstance =GetModuleHandleA(0);
    HICON icon = LoadIcon(hInstance, IDI_APPLICATION);
    WNDCLASSA wc;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "SampleWindowClass";

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        "SampleWindowClass",
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    return this;
}

void WindowsWindow::SetTitle(const char* title)
{
    this->title = const_cast<char*>(title);
    SetWindowText(hwnd, title);
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
    ::ShowWindow(hwnd, SW_SHOW);
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

void WindowsWindow::Update(const void* buffer, uint32_t width, uint32_t height)
{
    // Update the display buffer if necessary
}
#endif
