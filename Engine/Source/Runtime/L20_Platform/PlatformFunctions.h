#ifndef PLATFORM_PLATFORM_FUNCTIONS_H
#define PLATFORM_PLATFORM_FUNCTIONS_H
#include "L20_Platform/L37_Window/Functions.h"
#include "L0_Macro/Include.h"
b8 RT_Platform_Initialize(RT_MAIN_WINDOW& window, const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y);

void RT_Platform_Terminate(RT_MAIN_WINDOW& window);

b8 RT_Platform_PumpMessage(RT_MAIN_WINDOW& window);

#endif
