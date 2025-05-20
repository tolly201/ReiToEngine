#ifndef PLATFORM_HAL_CONLOLE_WINDOWS_CONSOLE_H
#define PLATFORM_HAL_CONLOLE_WINDOWS_CONSOLE_H
#include "../IConsole.h"

#ifdef RT_SYSTEM_WINDOWS

#include "windows.h"
void RT_HAL_ConsoleWrite(const char* message, u8 color);
void RT_HAL_ConsoleWriteError(const char* message, u8 color);
#endif
#endif
