#ifndef PLATFORM_WINDOW_FUNCTIONS_H
#define PLATFORM_WINDOW_FUNCTIONS_H
#include "WindowEnums.h"
#include "L0_Macro/Include.h"

#ifdef RT_SYSTEM_WINDOWS
    #include <windows.h>
#endif

struct RT_Platform_State
{
#ifdef RT_SYSTEM_WINDOWS
    HINSTANCE hInstance;
#endif
};

b8 RT_Platform_MainWindowInitialize(RT_Platform_State* platform_state);

void RT_Platform_MainWindowTerminate(RT_Platform_State* platform_state);

b8 RT_Platform_MainWindowPumpMessage(RT_Platform_State* platform_state);

#endif
