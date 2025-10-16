#ifndef PLATFORM_MEMORY_MANAGER_IMEMORYALLOCATOR_H
#define PLATFORM_MEMORY_MANAGER_IMEMORYALLOCATOR_H
#include "L0_Macro/Include.h"
#include "IBlock.h"
#include "L20_Platform/L21_Logger/Include.h"
#include "L20_Platform/L20_Memory/Include.h"
#include <cstring>
namespace ReiToEngine
{
struct MemoryStats
{
    u64 TotalUsedMemory = 0;
    u64 PeakUsedMemory = 0; // 运行期峰值（累计分配减去释放后达到的最高 TotalUsedMemory）
    u64 TaggedAllocatedMemory[static_cast<u16>(RT_MEMORY_TAG::MAX_TAG)] = {0};
    u64 TaggedPeakMemory[static_cast<u16>(RT_MEMORY_TAG::MAX_TAG)] = {0};
};

template <typename T>
class RTMemoryManager
{
public:
    RTMemoryManager() = default;
    virtual ~RTMemoryManager() = 0;
    void* Allocate(u64 size, u8 alignment, RT_MEMORY_TAG tag)
    {
        if (tag == RT_MEMORY_TAG::UNKNOWN)
        {
            RT_LOG_WARN_PLATFORM("RTMemoryManager::Allocate called with UNKNOWN tag. This may lead to memory leaks or undefined behavior.");
        }

        Stats.TotalUsedMemory += size;
        if(Stats.TotalUsedMemory > Stats.PeakUsedMemory) Stats.PeakUsedMemory = Stats.TotalUsedMemory;
        Stats.TaggedAllocatedMemory[static_cast<u16>(tag)] += size;
        void* block = static_cast<T*>(this)->AllocateImpl(size, alignment, tag);

        Stats.TaggedPeakMemory[static_cast<u16>(tag)] = RTMAX(Stats.TaggedPeakMemory[static_cast<u16>(tag)], Stats.TaggedAllocatedMemory[static_cast<u16>(tag)]);

        ZeroMemoryReiTo(block, size);
        return block;
    }
    void Free(void* addr, u64 size, RT_MEMORY_TAG tag)
    {
        if (tag == RT_MEMORY_TAG::UNKNOWN)
        {
            RT_LOG_WARN_PLATFORM("RTMemoryManager::Allocate called with UNKNOWN tag. This may lead to memory leaks or undefined behavior.");
        }

        Stats.TotalUsedMemory -= size;
        Stats.TaggedAllocatedMemory[static_cast<u16>(tag)] -= size;
        static_cast<T*>(this)->FreeImpl(addr, size, tag);
    }
    void* ZeroMemoryReiTo(void* addr, u64 size)
    {
        return static_cast<T*>(this)->ZeroMemoryImpl(addr, size);
    }
    void* CopyMemoryReiTo(void* dest, const void* src, u64 size)
    {
        return static_cast<T*>(this)->CopyMemoryImpl(dest, src, size);
    }
    void* SetMemory(void* addr, u8 value, u64 size)
    {
        return static_cast<T*>(this)->SetMemoryImpl(addr, value, size);
    }
    char* GetMemoryUsageReport()
    {
        static thread_local char buffer[8000];
        // 写入标题
        strcpy(buffer, "System memory use (tagged):\n");
        size_t offset = strlen(buffer);
        const size_t capacity = sizeof(buffer);
        auto format_amount = [](u64 bytes, char* outUnit)->double{
            double amount = (double)bytes;
            if(bytes >= GIGABYTE){ outUnit[0]='G'; outUnit[1]='B'; outUnit[2]='\0'; amount = (double)bytes / (double)GIGABYTE; }
            else if(bytes >= MEGABYTE){ outUnit[0]='M'; outUnit[1]='B'; outUnit[2]='\0'; amount = (double)bytes / (double)MEGABYTE; }
            else if(bytes >= KILOBYTE){ outUnit[0]='K'; outUnit[1]='B'; outUnit[2]='\0'; amount = (double)bytes / (double)KILOBYTE; }
            else { outUnit[0]='B'; outUnit[1]='\0'; }
            return amount;
        };
        if(offset < capacity){
            char u1[4], u2[4];
            double cur = format_amount(Stats.TotalUsedMemory, u1);
            double peak = format_amount(Stats.PeakUsedMemory, u2);
            size_t rem = capacity - offset;
            int written = snprintf(buffer + offset, rem, "TotalUsed: %.2f %s\nPeakUsed: %.2f %s\n", cur, u1, peak, u2);
            if(written>0){ offset += (size_t)written; if(offset>=capacity){ buffer[capacity-1]='\0'; return buffer; }}
        }

        for (u16 i = 0; i < static_cast<u16>(RT_MEMORY_TAG::MAX_TAG); ++i) {
            char unitCur[4]; char unitPeak[4];
            double cur = format_amount(Stats.TaggedAllocatedMemory[i], unitCur);
            double pk  = format_amount(Stats.TaggedPeakMemory[i], unitPeak);
            if (offset < capacity) {
                size_t remaining = capacity - offset;
                int written = snprintf(buffer + offset, remaining, "%s: %.2f %s (peak %.2f %s)\n", MemoryTags[i], cur, unitCur, pk, unitPeak);
                if (written > 0) {
                    offset += (size_t)written;
                    if (offset >= capacity) { buffer[capacity - 1] = '\0'; break; }
                } else { break; }
            } else { break; }
        }
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
        "UNKNOWN","CONTAINER" ,"ARRAY", "DARRAY", "DICT", "QUEUE", "BST", "STRING",
        "APPLICATION", "JOB", "TEXTURE", "MATERIAL_INSTANCE", "RENDERER",
        "GAME", "TRANSFORM", "ENTITY", "ENTITT_NODE", "SCENE", "AUDIO", "SINGLETON"
    };
};
template <typename T>
RTMemoryManager<T>::~RTMemoryManager() = default;
}
#endif
