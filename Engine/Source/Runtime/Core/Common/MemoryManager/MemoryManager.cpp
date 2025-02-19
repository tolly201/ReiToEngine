#include "IMemoryManager.h"
namespace ReiToEngine
{
template <typename T>
RTMemoryManager<T>::~RTMemoryManager() = default;

RTCMemoryManager::~RTCMemoryManager() = default;

void* RTCMemoryManager::allocateImpl(uint32_t size, uint32_t alignment)
{
	std::cout << "allocate" << size << std::endl;

	return std::malloc(size);
}

void RTCMemoryManager::deallocateImpl(void* addr, uint32_t alignment)
{
	std::free(addr);
}
}

// 在全局作用域强制替换
RT_FORCEINLINE void* operator new(size_t uiSize)
{
	std::cout << "allocate1";
    return ReiToEngine::RTCMemoryManager::GetInstance().Allocate(uiSize,4); 
}
RT_FORCEINLINE void* operator new[] (size_t uiSize)
{
	std::cout << "allocate2";
	return ReiToEngine::RTCMemoryManager::GetInstance().Allocate(uiSize,4); 
}

RT_FORCEINLINE void operator delete (void* pvAddr)noexcept
{
	std::cout << "dallocate1";
	return ReiToEngine::RTCMemoryManager::GetInstance().Deallocate(pvAddr, 4); 
}
RT_FORCEINLINE void operator delete[] (void* pvAddr)noexcept
{
	std::cout << "dallocate2";
	return ReiToEngine::RTCMemoryManager::GetInstance().Deallocate(pvAddr, 4); 
}

// 重载 malloc 和 free
// RT_FORCEINLINE void* malloc(size_t size)
// {
//     std::cout << "Custom malloc called, allocating " << size << " bytes\n";
//     // 你的自定义分配逻辑...
//     // return ReiToEngine::RTCMemoryManager::GetInstance().Allocate(size,4); //  或者你自己的底层分配机制
//     return nullptr;
// }

// RT_FORCEINLINE void free(void* ptr)
// {
//     std::cout << "Custom free called\n";
//     // 你的自定义释放逻辑...
//     // ReiToEngine::RTCMemoryManager::GetInstance().Deallocate(ptr, 4); //  或者你自己的底层分配机制
//     return;
// }