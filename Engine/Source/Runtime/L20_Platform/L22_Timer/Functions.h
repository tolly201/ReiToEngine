#ifndef PLATFORM_TIMER_FUNCTIONS_H
#define PLATFORM_TIMER_FUNCTIONS_H
#include "L0_Macro/Include.h"

f64 RT_Platform_AbsoluteTime();
void RT_Platform_InitTime();
void RT_Platform_Sleep(u64 ms);
#endif
