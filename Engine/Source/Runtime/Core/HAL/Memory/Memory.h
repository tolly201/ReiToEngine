#ifndef CORE_LAH_MEMORY_H
#define CORE_LAH_MEMORY_H
#include "../../InnerCore.h"

void* RT_HAL_SYSAlloc(size_t uiSize);
void RT_HAL_SYSFree(void* pMem, size_t uiSize);

#endif
