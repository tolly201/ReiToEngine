#include "Function.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
void* RT_Platform_SYSAlloc(u64 uiSize) {
    void* ptr = VirtualAlloc(nullptr, uiSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    // if (ptr == nullptr)
    // {
    //     return nullptr;
    // }
    return ptr;
    return VirtualAlloc(nullptr, uiSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void RT_Platform_SYSFree(void* pMem, u64 uiSize) {
    // if (pMem != nullptr)
    // {
    VirtualFree(pMem, 0, MEM_RELEASE);
    // }
}

void RT_Platform_SYSZeroMemory(void* pMem, u64 uiSize) {
    if (pMem && uiSize > 0) {
        ZeroMemory(pMem, uiSize);
    }
}

void RT_Platform_SYSCopyMemory(void* pDest, const void* pSrc, u64 uiSize) {
    if (pDest && pSrc && uiSize > 0) {
        CopyMemory(pDest, pSrc, uiSize);
    }
}

void RT_Platform_SYSMoveMemory(void* pDest, const void* pSrc, u64 uiSize, u8 alignment, b8 aligned, b8 isArray) {
    if (pDest && pSrc && uiSize > 0) {
        MoveMemory(pDest, pSrc, uiSize);
    }
}

void RT_Platform_SYSSetMemory(void* pMem, u8 value, u64 uiSize) {
    if (pMem && uiSize > 0) {
        FillMemory(pMem, uiSize, value);
    }
}
#endif
