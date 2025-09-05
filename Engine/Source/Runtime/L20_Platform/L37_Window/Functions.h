#ifndef PLATFORM_WINDOW_FUNCTIONS_H
#define PLATFORM_WINDOW_FUNCTIONS_H
#include "WindowEnums.h"
#include "L0_Macro/Include.h"

struct RT_Platform_State;

b8 RT_Platform_MainWindowInitialize(RT_Platform_State* platform_state);

void RT_Platform_MainWindowTerminate(RT_Platform_State* platform_state);

b8 RT_Platform_MainWindowPumpMessage(RT_Platform_State* platform_state);

#endif
