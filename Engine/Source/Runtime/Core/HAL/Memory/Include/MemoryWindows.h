#ifndef CORE_HAL_MEMORY_WINDOWS_H
#define CORE_HAL_MEMORY_WINDOWS_H
#include "../Memory.h"
#ifdef RT_SYSTEM_WINDOWS
#include <cstddef>
#include <iostream>
#include <Windows.h>

void* RT_HAL_SYSAlloc(size_t uiSize);
void RT_HAL_SYSFree(void* pMem, size_t uiSize);

#endif
#endif
