#ifndef PLATFORM_MEMORY_FUNCTIONS_H
#define PLATFORM_MEMORY_FUNCTIONS_H
#include "L0_Macro/Include.h"

// NOTE:
// MSVC 在某些组合下(特别是开启 /Zc:preprocessor 标准预处理并含有指针返回类型)对
// 形如: "void* __declspec(dllexport) Function(...)" 的写法会报 C2059 语法错误。
// 官方推荐以及广泛兼容的形式为: "__declspec(dllexport) void* Function(...)"。
// 因此将 RTENGINE_API 统一放置到返回类型之前，兼容所有主流编译器/模式。

RTENGINE_API void* RT_Platform_SYSAlloc(u64 uiSize);

RTENGINE_API void RT_Platform_SYSFree(void* pMem, u64 uiSize);

RTENGINE_API void RT_Platform_SYSZeroMemory(void* pMem, u64 uiSize);

RTENGINE_API void RT_Platform_SYSCopyMemory(void* pDest, const void* pSrc, u64 uiSize);

RTENGINE_API void RT_Platform_SYSMoveMemory(void* pDest, const void* pSrc, u64 uiSize);

RTENGINE_API void RT_Platform_SYSSetMemory(void* pMem,u8 value, u64 uiSize);
#endif
