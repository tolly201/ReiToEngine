#include "MemoryManager.h"
namespace ReiToEngine
{
template <typename T>
RTMemoryManager<T>::~RTMemoryManager() = default;  // 这里提供了析构函数的定义

RTCMemoryManager::~RTCMemoryManager() = default;

void* RTCMemoryManager::allocateImpl(uint32_t size, uint32_t alignment)
{
	if (!size)
	{
		return nullptr;
	}
	if (alignment == 0)
	{
		return std::malloc(size);
	}
	else
	{
		return std::aligned_alloc(alignment, size);
	}
	return nullptr;
}

void RTCMemoryManager::deallocateImpl(void* addr, uint32_t size, uint32_t alignment)
{
	if (addr == nullptr)
	{
		return;
	}

	if (alignment == 0)
	{
		free(addr);
	}
	else
	{
		return std::free(addr);
	}
}

}