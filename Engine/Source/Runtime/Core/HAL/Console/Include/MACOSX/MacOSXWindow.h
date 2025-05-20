#ifndef PLATFORM_HAL_CONLOLE_MACOSX_CONSOLE_H
#define PLATFORM_HAL_CONLOLE_MACOSX_CONSOLE_H

#include "../IConsole.h"

#ifdef RT_SYSTEM_APPLE

void RT_HAL_ConsoleWrite(const char* message, u8 color);
void RT_HAL_ConsoleWriteError(const char* message, u8 color);
#endif
#endif
