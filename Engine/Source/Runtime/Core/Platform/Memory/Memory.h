#include "./Include/RTDebugHeapMemoryManager.h"
#include <iostream>
#include <memory>

// 在全局作用域强制替换
RT_FORCEINLINE void* operator new(size_t uiSize)
{
    return ReiToEngine::RTDebugHeapMemoryManager::Instance().Allocate(uiSize, 8, false);
}
RT_FORCEINLINE void* operator new[] (size_t uiSize)
{
	return ReiToEngine::RTDebugHeapMemoryManager::Instance().Allocate(uiSize,8, true);
}

RT_FORCEINLINE void operator delete (void* pvAddr)noexcept
{
	return ReiToEngine::RTDebugHeapMemoryManager::Instance().Deallocate(pvAddr, 8, false);
}
RT_FORCEINLINE void operator delete[] (void* pvAddr)noexcept
{
	return ReiToEngine::RTDebugHeapMemoryManager::Instance().Deallocate(pvAddr, 8, true);
}

// 重载 malloc 和 free
RT_FORCEINLINE void* malloc(size_t size)
{
    return ReiToEngine::RTDebugHeapMemoryManager::Instance().Allocate(size, 8, false);
}

RT_FORCEINLINE void free(void* ptr)
{
    ReiToEngine::RTDebugHeapMemoryManager::Instance().Deallocate(ptr, 8, false);
    return;
}
