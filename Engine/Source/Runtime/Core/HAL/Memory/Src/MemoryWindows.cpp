#include "../Include/MemoryWindows.h"
#ifdef RT_SYSTEM_WINDOWS
#include <cstddef>
#include <iostream>
#include <Windows.h>
void* RT_HAL_SYSAlloc(u64 uiSize)
{
    void* ptr = VirtualAlloc(nullptr, uiSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (ptr == nullptr)
    {
        return nullptr;
    }
    return ptr;
}

void RT_HAL_SYSFree(void* pMem, u64 uiSize)
{
    if (pMem != nullptr)
    {
        VirtualFree(pMem, 0, MEM_RELEASE);
    }
}

#endif
