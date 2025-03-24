#include "Core.h"
#include "Core/Application/Include/RTApplication.h"


ReiToEngine::RTApplication ReiToEngine::RTApplication::instance;

void* operator new(size_t uiSize)
{
    return ReiToEngine::RTApplication::Instance().GetMemoryManager().Allocate(uiSize, 8, false);
}
void* operator new[] (size_t uiSize)
{
	return ReiToEngine::RTApplication::Instance().GetMemoryManager().Allocate(uiSize,8, true);
}

void operator delete (void* pvAddr)noexcept
{
	return ReiToEngine::RTApplication::Instance().GetMemoryManager().Deallocate(pvAddr, 8, false);
}

void operator delete[] (void* pvAddr)noexcept
{
	return ReiToEngine::RTApplication::Instance().GetMemoryManager().Deallocate(pvAddr, 8, true);
}

// 重载 malloc 和 free
RT_FORCEINLINE void* malloc(size_t size)
{
    return ReiToEngine::RTApplication::Instance().GetMemoryManager().Allocate(size, 8, false);
}

RT_FORCEINLINE void free(void* ptr)
{
    return ReiToEngine::RTApplication::Instance().GetMemoryManager().Deallocate(ptr, 8, false);
}
