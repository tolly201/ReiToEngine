#include "RTCMemoryManager.h"
namespace ReiToEngine
{
void* RTCMemoryManager::AllocateImpl(u64 size,[[maybe_unused]] u8 alignment,[[maybe_unused]] RT_MEMORY_TAG tag)
{
    return RT_Platform_SYSAlloc(size);
}
void RTCMemoryManager::FreeImpl([[maybe_unused]]void* addr, [[maybe_unused]]u64 size, [[maybe_unused]]RT_MEMORY_TAG tag)
{
    return RT_Platform_SYSFree(addr, 0);
}

void* RTCMemoryManager::ZeroMemoryImpl(void* addr, u64 size)
{
    if (addr && size > 0) {
        RT_Platform_SYSZeroMemory(addr, size);
    }
    return addr;
}
void* RTCMemoryManager::CopyMemoryImpl(void* dest, const void* src, u64 size)
{
    if (dest && src && size > 0) {
        RT_Platform_SYSCopyMemory(dest, src, size);
    }
    return dest;
}
void* RTCMemoryManager::SetMemoryImpl(void* addr, u8 value, u64 size)
{
    if (addr && size > 0) {
        RT_Platform_SYSSetMemory(addr, value, size);
    }
    return addr;
}
}

