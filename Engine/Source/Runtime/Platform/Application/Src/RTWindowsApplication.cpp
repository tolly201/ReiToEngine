#include "../Include/RTWindowsApplication.h"

#ifdef RT_SYSTEM_WINDOWS

using namespace ReiToEngine;
void RTWindowsApplication::Initialize(ApplicatonConfig& app_config)
{
    RTApplication::Initialize(app_config);
    hInstance = GetModuleHandleW(0);
    // Setup and register window class. This can be done at platform init and be reused.
    HICON icon = LoadIconW(hInstance, IDI_APPLICATION);
    WNDCLASSEXW wc = {0};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_DBLCLKS; // Get double-clicks
    // wc.lpfnWndProc = win32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = icon;
    wc.hIconSm = icon;
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW); // NULL; // Manage the cursor manually
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = L"kohi_window_class";
    wc.lpszMenuName = 0;

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
            RTFATAL(message_buf);

            delete[] message_buf;
            LocalFree(wmessage_buf);
        } else {
            MessageBoxW(0, L"Window registration failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
            RTFATAL("Window registration failed!");
        }
    }
}
void RTWindowsApplication::Run()
{
    RTApplication::Run();
}
void RTWindowsApplication::Tick()
{
    RTApplication::Tick();
}
void RTWindowsApplication::Terminate()
{

    RTApplication::Terminate();
    // Unregister the window class
    UnregisterClassW(L"kohi_window_class", hInstance);
}
#endif
