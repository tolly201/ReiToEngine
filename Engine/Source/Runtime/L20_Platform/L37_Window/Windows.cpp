#include "Functions.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
#include "L20_Platform/L23_Logger/Include.h"
#include "./Windows/WindowsWindow.h"
b8 RT_Platform_MainWindowInitialize()
{
    return true;
}

void RT_Platform_MainWindowTerminate()
{
}

b8 RT_Platform_MainWindowPumpMessage()
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
