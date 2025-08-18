#ifndef PLATFORM_WINDOW_FUNCTIONS_H
#define PLATFORM_WINDOW_FUNCTIONS_H
#include "WindowEnums.h"
#include "L0_Macro/Include.h"
b8 RT_Platform_MainWindowInitialize();

void RT_Platform_MainWindowTerminate();

b8 RT_Platform_MainWindowPumpMessage();

#endif
