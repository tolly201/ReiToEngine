#include "../Include/RTCMemoryManager.h"
#include <memory>

namespace ReiToEngine
{
RTCMemoryManager::~RTCMemoryManager() = default;
void* RTCMemoryManager::allocateImpl(size_t size, uint8_t alignment, bool isArray)
{
    return ::malloc(size);
}
void RTCMemoryManager::deallocateImpl(void* addr, uint8_t alignment, bool isArray)
{
    return ::free(addr);
}
}

