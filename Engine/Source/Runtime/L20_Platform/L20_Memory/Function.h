#ifndef PLATFORM_MEMORY_FUNCTIONS_H
#define PLATFORM_MEMORY_FUNCTIONS_H
#include "L0_Macro/Include.h"

void* RT_Platform_SYSAlloc(u64 uiSize);

void RT_Platform_SYSFree(void* pMem, u64 uiSize);

void RT_Platform_SYSZeroMemory(void* pMem, u64 uiSize);

void RT_Platform_SYSCopyMemory(void* pDest, const void* pSrc, u64 uiSize);

void RT_Platform_SYSMoveMemory(void* pDest, const void* pSrc, u64 uiSize);

void RT_Platform_SYSSetMemory(void* pMem,u8 value, u64 uiSize);
#endif
