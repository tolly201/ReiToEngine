#ifndef PLATFORM_WINDOW_FUNCTIONS_H
#define PLATFORM_WINDOW_FUNCTIONS_H
#include "WindowEnums.h"
#include "L0_Macro/Include.h"
struct RT_MAIN_WINDOW
{
    IWindow* main_window = nullptr;
};

b8 RT_Platform_CreateMainWindow(RT_MAIN_WINDOW& window, const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y);

void RT_Platform_CloseMainWindow(RT_MAIN_WINDOW& window);

b8 RT_Platform_MainWindowPumpMessage(RT_MAIN_WINDOW& window);

#endif
