#include "../Include/RTCMemoryManager.h"
#include <memory>
#include <cstdlib>
namespace ReiToEngine
{
RTCMemoryManager::~RTCMemoryManager() = default;
void* RTCMemoryManager::allocateImpl(size_t size, uint8_t alignment, bool isArray)
{
    return std::malloc(size);
}
void RTCMemoryManager::deallocateImpl(void* addr, uint8_t alignment, bool isArray)
{
    return std::free(addr);
}
}

