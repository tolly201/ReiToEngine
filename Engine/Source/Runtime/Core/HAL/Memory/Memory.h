#ifndef CORE_LAH_MEMORY_H
#define CORE_LAH_MEMORY_H
#include "../../InnerCore.h"

void* RT_HAL_SYSAlloc(u64 uiSize);
void RT_HAL_SYSFree(void* pMem, u64 uiSize);

#endif
