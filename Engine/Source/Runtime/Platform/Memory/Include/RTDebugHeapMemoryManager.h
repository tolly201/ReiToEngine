#ifndef CORE_HAL_MEMORY_RT_LOG_DEBUGSTACKMEMORYALLOCATOR_H
#define CORE_HAL_MEMORY_RT_LOG_DEBUGSTACKMEMORYALLOCATOR_H
#include "IBlock.h"
#include "IMemoryManager.h"
#include <mutex>
#include <cstddef> // for std::max_align_t

namespace ReiToEngine
{
class RTDebugHeapMemoryManager : public RTMemoryManager<RTDebugHeapMemoryManager>

{
    friend class RTMemoryManager<RTDebugHeapMemoryManager>;
public:
    RTDebugHeapMemoryManager();
    ~RTDebugHeapMemoryManager();
    void TrackAllocation(void* Ptr, size_t Size, const char* File, uint32_t Line);
    void UntrackAllocation(void* Ptr);
    void DumpMemoryLeaks();
protected:
    void* AllocateImpl(u64, u8, RT_MEMORY_TAG) override;
    void FreeImpl(void*, u64, RT_MEMORY_TAG) override;
    void* ZeroMemoryImpl(void*, u64) override;
    void* CopyMemoryImpl(void*, const void*, u64) override;
    void* SetMemoryImpl(void*, u8, u64) override;
private:
    // RTDebugHeapMemoryManager(const RTDebugHeapMemoryManager&) = delete;
    // RTDebugHeapMemoryManager& operator=(const RTDebugHeapMemoryManager&) = delete;
    void* allocatePoolCall(uint32_t, uint32_t, bool isArray);
    void deAllocatePoolCall(void*, uint32_t, bool isArray);
    void* allocateSystemCall(uint32_t, uint32_t, bool isArray);
    void deAllocateSystemCall(void*, uint32_t, bool isArray);
    //Memory For Small Object, new and delete often
    RTBlock* HeadSmallTarget;
    RTBlock* TailSmallTarget;

    size_t TotalAllocatedMemory;
    size_t PeakAllocatedMemory;
    const static size_t alignedRTBlockSize = (sizeof(RTBlock) + 7) & ~7;

    std::mutex MemoryMutex;
public:
};
}

#endif
