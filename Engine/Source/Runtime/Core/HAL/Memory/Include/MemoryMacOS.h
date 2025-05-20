#ifndef CORE_HAL_MEMORY_MACOS_H
#define CORE_HAL_MEMORY_MACOS_H
#include "../Memory.h"
#ifdef RT_SYSTEM_APPLE
void* RT_HAL_SYSAlloc(u64 uiSize);
void RT_HAL_SYSFree(void* pMem, u64 uiSize);
#endif
#endif
