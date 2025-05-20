#ifndef CORE_HAL_MEMORY_WINDOWS_H
#define CORE_HAL_MEMORY_WINDOWS_H
#include "../Memory.h"
#ifdef RT_SYSTEM_WINDOWS
#include <cstddef>
#include <iostream>
#include <Windows.h>

void* RT_HAL_SYSAlloc(u64 uiSize);
void RT_HAL_SYSFree(void* pMem, u64 uiSize);

#endif
#endif
