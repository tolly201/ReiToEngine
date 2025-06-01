#include "../PlatformDefines.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
#include "../Window/Include/Windows/WindowsWindow.h"
namespace {
    LRESULT CALLBACK win32_process_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
        switch (msg)
        {
            case WM_ERASEBKGND:
                return 1;
            case WM_CLOSE:
            //TODO: Fire an event for the application layer
                return 0;
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
            case WM_SIZE:
            {
                RECT r;
                GetClientRect(hwnd, &r);
                u32 width = r.right - r.left;
                u32 height = r.bottom - r.top;
            }
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                break;
            }
            case WM_MOUSEMOVE:
            {
                GET_DEVICE_LPARAM(lparam);
                break;
            }
            case WM_MOUSEWHEEL:
            {
                i32 z_delta = GET_WHEEL_DELTA_WPARAM(wparam);
            }
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP:
            default:
            {}
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }


    void RegisterBaseWindowClass(HINSTANCE& hInstance)
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
        wc.lpszClassName = L"rt_base_window_class";
        wc.lpszMenuName = 0;
        wc.lpfnWndProc = win32_process_message;

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

b8 RT_HAL_Initialize(RT_HAL_MAIN_WINDOW& window, const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y)
{
    HINSTANCE hInstance = GetModuleHandleW(0);
    RegisterBaseWindowClass(hInstance);
    window.main_window = new WindowsWindow();
    window.main_window->Create(title, width, height, pos_x, pos_y);
    window.main_window->ShowWindow();
}

void RT_HAL_Terminate(RT_HAL_MAIN_WINDOW& window)
{
    if (window.main_window != nullptr)
    {
        window.main_window->CloseWindow();
        delete window.main_window;
        window.main_window = nullptr;
    }
    UnregisterClassW(L"rt_base_window_class", GetModuleHandleW(0));
}

b8 RT_HAL_PumpMessage(RT_HAL_MAIN_WINDOW& window)
{
    MSG message;
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return true;
}

void* RT_HAL_SYSAlloc(u64 uiSize)
{
    void* ptr = VirtualAlloc(nullptr, uiSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    // if (ptr == nullptr)
    // {
    //     return nullptr;
    // }
    return ptr;
    return VirtualAlloc(nullptr, uiSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void RT_HAL_SYSFree(void* pMem, u64 uiSize)
{
    // if (pMem != nullptr)
    // {
        VirtualFree(pMem, 0, MEM_RELEASE);
    // }
}

void RT_HAL_SYSZeroMemory(void* pMem, u64 uiSize, u8 alignment, b8 aligned, b8 isArray)
{
    if (pMem && uiSize > 0) {
        ZeroMemory(pMem, uiSize);
    }
}

void RT_HAL_SYSCopyMemory(void* pDest, const void* pSrc, u64 uiSize, u8 alignment, b8 aligned, b8 isArray)
{
    if (pDest && pSrc && uiSize > 0) {
        CopyMemory(pDest, pSrc, uiSize);
    }
}

void RT_HAL_SYSMoveMemory(void* pDest, const void* pSrc, u64 uiSize, u8 alignment, b8 aligned, b8 isArray)
{
    if (pDest && pSrc && uiSize > 0) {
        MoveMemory(pDest, pSrc, uiSize);
    }
}

void RT_HAL_SYSSetMemory(void* pMem,u8 value, u64 uiSize, u8 alignment, b8 aligned, b8 isArray)
{
    if (pMem && uiSize > 0) {
        FillMemory(pMem, uiSize, value);
    }
}

void RT_HAL_ConsoleWrite(const char* message, u8 color)
{
    static u8 color_levels[6] = {0x0c | 0x40, 0x0c, 0x0E, 0x0A, 0x0B, 0x07};
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console_handle, &csbi);
    WORD original_attributes = csbi.wAttributes;

    SetConsoleTextAttribute(console_handle, color_levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, nullptr);
    SetConsoleTextAttribute(console_handle, original_attributes);
}

void RT_HAL_ConsoleWriteError(const char* message, u8 color)
{
    static u8 color_levels[6] = {0x0c | 0x40, 0x0c, 0x0E, 0x0A, 0x0B, 0x07};
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console_handle, &csbi);
    WORD original_attributes = csbi.wAttributes;

    SetConsoleTextAttribute(console_handle, color_levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, nullptr);

    SetConsoleTextAttribute(console_handle, original_attributes);
}

// f64 RT_HAL_AbsoluteTime();
// void RT_HAL_InitTime();
void RT_HAL_Sleep(u64 ms)
{
    Sleep(ms);
}


#endif
