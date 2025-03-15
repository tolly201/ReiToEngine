#ifndef CORE_HAL_MEMORY_RTDEBUGSTACKMEMORYALLOCATOR_H
#define CORE_HAL_MEMORY_RTDEBUGSTACKMEMORYALLOCATOR_H
#include "IBlock.h"
#include "IMemoryManager.h"
#include <mutex>
#include <cstddef> // for std::max_align_t

namespace ReiToEngine
{
class RTENGINE_API RTDebugHeapMemoryManager : public RTMemoryManager<RTDebugHeapMemoryManager>
{
    friend class RTMemoryManager<RTDebugHeapMemoryManager>;
public:
    RTDebugHeapMemoryManager();
    ~RTDebugHeapMemoryManager();
    void TrackAllocation(void* Ptr, size_t Size, const char* File, uint32_t Line);
    void UntrackAllocation(void* Ptr);
    void DumpMemoryLeaks();
protected:
    void* allocateImpl(size_t, uint8_t, bool) override;
    void deallocateImpl(void*, uint8_t, bool) override;
private:

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
