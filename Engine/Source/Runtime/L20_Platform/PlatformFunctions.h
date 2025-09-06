#ifndef PLATFORM_PLATFORM_FUNCTIONS_H
#define PLATFORM_PLATFORM_FUNCTIONS_H
#include "L20_Platform/L37_Window/Functions.h"
#include "L0_Macro/Include.h"

b8 RT_Platform_Initialize(RT_Platform_State*& platform_state);

void RT_Platform_Terminate(RT_Platform_State*& platform_state);

b8 RT_Platform_PumpMessage(RT_Platform_State*& platform_state);

#endif
