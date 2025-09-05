#include "Functions.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
#include "L20_Platform/L23_Logger/Include.h"
#include "WindowsEnums.h"
namespace{
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Handle window messages here
    PlatformInputMonitor* monitor = reinterpret_cast<PlatformInputMonitor*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (monitor) {
        MSG msg = { hwnd, uMsg, wParam, lParam };
        monitor->ProcessNativeEvent(&msg);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

    void RegisterBaseWindowClass(HINSTANCE& hInstance,const wchar_t* class_name)
    {
        HICON icon = LoadIconW(hInstance, IDI_APPLICATION);
        WNDCLASSEXW wc = {0};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_DBLCLKS; // Get double-clicks
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = icon;
        wc.hIconSm = icon;
        wc.hCursor = LoadCursorW(NULL, IDC_ARROW); // NULL; // Manage the cursor manually
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszClassName = class_name;
        wc.lpszMenuName = 0;
        wc.lpfnWndProc = WindowProc;

        if (!RegisterClassExW(&wc)) {
            DWORD last_error = GetLastError();
            LPWSTR wmessage_buf = 0;

            // Ask Win32 to give us the string version of that message ID.
            // The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
            u64 size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, last_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&wmessage_buf, 0, NULL);
            if (size) {
                int mb_len = WideCharToMultiByte(CP_UTF8, 0, wmessage_buf, -1, NULL, 0, NULL, NULL);
                char* message_buf = new char[mb_len];
                WideCharToMultiByte(CP_UTF8, 0, wmessage_buf, -1, message_buf, mb_len, NULL, NULL);

                MessageBoxW(NULL, wmessage_buf, L"Error!", MB_ICONEXCLAMATION | MB_OK);
                    RT_LOG_FATAL(message_buf);

                delete[] message_buf;
                LocalFree(wmessage_buf);
            } else {
                MessageBoxW(0, L"Window registration failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
                RT_LOG_FATAL("Window registration failed!");
            }
        }
    }
}


b8 RT_Platform_MainWindowInitialize(RT_Platform_State* platform_state)
{
    HINSTANCE hInstance = platform_state->hInstance;
    RegisterBaseWindowClass(hInstance, L"rt_base_window_class");
    return true;
}

void RT_Platform_MainWindowTerminate(RT_Platform_State* platform_state)
{
    HINSTANCE hInstance = plaform_state->hInstance;
    UnregisterClassW(L"rt_base_window_class", hInstance);
}

b8 RT_Platform_MainWindowPumpMessage(RT_Platform_State* platform_state)
{
    MSG message;
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return true;
}
#endif
