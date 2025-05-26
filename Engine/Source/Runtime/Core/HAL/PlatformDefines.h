#ifndef RT_HAL_PLATFORM_DEFINES_H
#define RT_HAL_PLATFORM_DEFINES_H
#include "./Window/Include/WindowEnums.h"
#include "../InnerCore.h"
struct RT_HAL_MAIN_WINDOW
{
    IWindow* main_window = nullptr;
};

b8 RT_HAL_Initialize(RT_HAL_MAIN_WINDOW& window, const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y);

void RT_HAL_Terminate(RT_HAL_MAIN_WINDOW& window);

b8 RT_HAL_PumpMessage(RT_HAL_MAIN_WINDOW& window);

void* RT_HAL_SYSAlloc(u64 uiSize);

void RT_HAL_SYSFree(void* pMem, u64 uiSize);

void RT_HAL_SYSZeroMemory(void* pMem, u64 uiSize);

void RT_HAL_SYSCopyMemory(void* pDest, const void* pSrc, u64 uiSize);

void RT_HAL_SYSMoveMemory(void* pDest, const void* pSrc, u64 uiSize);

void RT_HAL_SYSSetMemory(void* pMem,u8 value, u64 uiSize);

void RT_HAL_ConsoleWrite(const char* message, u8 color);

void RT_HAL_ConsoleWriteError(const char* message, u8 color);

f64 RT_HAL_AbsoluteTime();
void RT_HAL_InitTime();
void RT_HAL_Sleep(u64 ms);
#endif
