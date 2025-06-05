#ifndef PLATFORM_MEMORY_MANAGER_IMEMORYALLOCATOR_H
#define PLATFORM_MEMORY_MANAGER_IMEMORYALLOCATOR_H
#include "L0_Macro/Include.h"
#include "IBlock.h"
#include "L20_Platform/L23_Logger/Include.h"
#include "L20_Platform/L20_Memory/Include.h"
#include <cstring>
namespace ReiToEngine
{
struct MemoryStats
{
    u64 TotalUsedMemory = 0;
    u64 TaggedAllocatedMemory[static_cast<u16>(RT_MEMORY_TAG::MAX_TAG)] = {0};
};

template <typename T>
class RTENGINE_API RTMemoryManager
{
public:
    RTMemoryManager() = default;
    virtual ~RTMemoryManager() = 0;
    void* Allocate(u64 size, u8 alignment, RT_MEMORY_TAG tag)
    {
        if (tag == RT_MEMORY_TAG::UNKNOWN)
        {
            RT_LOG_WARN("RTMemoryManager::Allocate called with UNKNOWN tag. This may lead to memory leaks or undefined behavior.");
        }

        Stats.TotalUsedMemory += size;
        Stats.TaggedAllocatedMemory[tag] += size;
        void* block = static_cast<T*>(this)->AllocateImpl(size, alignment, tag);
        ZeroMemory(block, size);
        return block;
    }
    void Free(void* addr, u64 size, RT_MEMORY_TAG tag)
    {
        if (tag == RT_MEMORY_TAG::UNKNOWN)
        {
            RT_LOG_WARN("RTMemoryManager::Allocate called with UNKNOWN tag. This may lead to memory leaks or undefined behavior.");
        }

        Stats.TotalUsedMemory -= size;
        Stats.TaggedAllocatedMemory[tag] -= size;
        static_cast<T*>(this)->FreeImpl(addr, size, tag);
    }
    void* ZeroMemory(void* addr, u64 size)
    {
        return static_cast<T*>(this)->ZeroMemoryImpl(addr, size);
    }
    void* CopyMemory(void* dest, const void* src, u64 size)
    {
        return static_cast<T*>(this)->CopyMemoryImpl(dest, src, size);
    }
    void* SetMemory(void* addr, u8 value, u64 size)
    {
        return static_cast<T*>(this)->SetMemoryImpl(addr, value, size);
    }
    char* GetMemoryUsageReport()
    {
        char buffer[8000] = "System memory use (tagged):\n";
        u64 offset = strlen(buffer);

        for (u16 i = 0; i < static_cast<u16>(RT_MEMORY_TAG::MAX_TAG); ++i)
        {
            char unit[3] = "XB";
            float amount = .0f;
            if (Stats.TaggedAllocatedMemory[i] >= GIGABYTE)
            {
                unit[0] = 'G';
                amount = Stats.TaggedAllocatedMemory[i] / GIGABYTE;
            }
            else if (Stats.TaggedAllocatedMemory[i] >= MEGABYTE)
            {
                unit[0] = 'M';
                amount = Stats.TaggedAllocatedMemory[i] / MEGABYTE;
            }
            else if (Stats.TaggedAllocatedMemory[i] >= KILOBYTE)
            {
                unit[0] = 'K';
                amount = Stats.TaggedAllocatedMemory[i] / KILOBYTE;
            }
            else
            {
                unit[0] = 'B';
                unit[1] = '\0';
                amount = Stats.TaggedAllocatedMemory[i];
            }
            i32 length = snprintf(buffer + offset, 8000, "%s: %.2f %s\n", MemoryTags[i], amount, unit);
            offset += length;
        }
        // char* report = _strdup(buffer);
        return buffer;
    }
protected:
    virtual void* AllocateImpl(u64, u8, RT_MEMORY_TAG) = 0;
    virtual void FreeImpl(void*, u64, RT_MEMORY_TAG) = 0;
    virtual void* ZeroMemoryImpl(void*, u64) = 0;
    virtual void* CopyMemoryImpl(void*, const void*, u64) = 0;
    virtual void* SetMemoryImpl(void*, u8, u64) = 0;
    MemoryStats Stats;

    const char* MemoryTags[static_cast<u16>(RT_MEMORY_TAG::MAX_TAG)] = {
        "UNKNOWN", "ARRAY", "DARRAY", "DICT", "QUEUE", "BST", "STRING",
        "APPLICATION", "JOB", "TEXTURE", "MATERIAL_INSTANCE", "RENDERER",
        "GAME", "TRANSFORM", "ENTITY", "ENTITT_NODE", "SCENE", "AUDIO"
    };
};
template <typename T>
RTMemoryManager<T>::~RTMemoryManager() = default;
}
#endif
