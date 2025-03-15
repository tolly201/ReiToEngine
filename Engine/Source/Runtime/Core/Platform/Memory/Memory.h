#include "./Include/RTDebugHeapMemoryManager.h"
#include <iostream>
#include <memory>

ReiToEngine::RTDebugHeapMemoryManager memManager;
// 在全局作用域强制替换
RT_FORCEINLINE void* operator new(size_t uiSize)
{
    return memManager.Allocate(uiSize, 8, false);
}
RT_FORCEINLINE void* operator new[] (size_t uiSize)
{
	return memManager.Allocate(uiSize,8, true);
}

RT_FORCEINLINE void operator delete (void* pvAddr)noexcept
{
	return memManager.Deallocate(pvAddr, 8, false);
}
RT_FORCEINLINE void operator delete[] (void* pvAddr)noexcept
{
	return memManager.Deallocate(pvAddr, 8, true);
}

// 重载 malloc 和 free
RT_FORCEINLINE void* malloc(size_t size)
{
    return memManager.Allocate(size, 8, false);
}

RT_FORCEINLINE void free(void* ptr)
{
    memManager.Deallocate(ptr, 8, false);
    return;
}
