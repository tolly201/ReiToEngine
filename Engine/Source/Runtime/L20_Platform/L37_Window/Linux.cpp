#include "Functions.h"
#ifdef RT_SYSTEM_LINUX
#include "./Include/Linux/Wayland/WaylandWindow.h"


b8 RT_Platform_CreateMainWindow(RT_Platform_MAIN_WINDOW& window, const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y)
{
    window.main_window = new WayLandWindow();
    window.main_window->Create(title, width, height, pos_x, pos_y);
    window.main_window->ShowWindow();
    return true;
}

void RT_Platform_CloseMainWindow(RT_Platform_MAIN_WINDOW& window)
{
    if (window.main_window != nullptr)
    {
        window.main_window->CloseWindow();
        delete window.main_window;
        window.main_window = nullptr;
    }
}

b8 RT_Platform_MainWindowPumpMessage(RT_Platform_MAIN_WINDOW& window)
{}

#endif
