#ifndef PLATFORM_CONSOLE_FUNCTIONS_H
#define PLATFORM_CONSOLE_FUNCTIONS_H
#include "L0_Macro/Include.h"
void RTENGINE_API RT_Platform_ConsoleWrite(const char* message, u8 color);
void RTENGINE_API RT_Platform_ConsoleWriteError(const char* message, u8 color);
#endif
