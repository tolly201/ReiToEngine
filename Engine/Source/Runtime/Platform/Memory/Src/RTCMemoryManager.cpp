#include "../Include/RTCMemoryManager.h"
namespace ReiToEngine
{
void* RTCMemoryManager::AllocateImpl(u64 size, u8 alignment, RT_MEMORY_TAG tag)
{
    return RT_HAL_SYSAlloc(size);
}
void RTCMemoryManager::FreeImpl(void* addr, u64 size, RT_MEMORY_TAG tag)
{
    return RT_HAL_SYSFree(addr, 0);
}

void* RTCMemoryManager::ZeroMemoryImpl(void* addr, u64 size)
{
    if (addr && size > 0) {
        RT_HAL_SYSZeroMemory(addr, size);
    }
    return addr;
}
void* RTCMemoryManager::CopyMemoryImpl(void* dest, const void* src, u64 size)
{
    if (dest && src && size > 0) {
        RT_HAL_SYSCopyMemory(dest, src, size);
    }
    return dest;
}
void* RTCMemoryManager::SetMemoryImpl(void* addr, u8 value, u64 size)
{
    if (addr && size > 0) {
        RT_HAL_SYSSetMemory(addr, value, size);
    }
    return addr;
}
}

