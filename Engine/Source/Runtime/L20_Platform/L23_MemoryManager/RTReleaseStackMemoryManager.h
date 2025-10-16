#ifndef CORE_HAL_MEMORY_RT_LOG_DEBUG_PLATFORMSTACKMEMORYALLOCATOR_H
#define CORE_HAL_MEMORY_RT_LOG_DEBUG_PLATFORMSTACKMEMORYALLOCATOR_H
#include "IBlock.h"
#include "IMemoryManager.h"
#include <mutex>
#include <cstddef> // for std::max_align_t

namespace ReiToEngine
{
class RTENGINE_API RTDebugStackMemoryManager : public RTMemoryManager<RTDebugStackMemoryManager>
{
    friend class RTMemoryManager<RTDebugStackMemoryManager>;
public:
    RTDebugStackMemoryManager();
    ~RTDebugStackMemoryManager();
    RTDebugStackMemoryManager& Instance();

    void TrackAllocation(void* Ptr, size_t Size, const char* File, uint32_t Line);
    void UntrackAllocation(void* Ptr);
    void DumpMemoryLeaks();

protected:
    void* AllocateImpl(u64, u8, RT_MEMORY_TAG) override;
    void FreeImpl(void*, u64, RT_MEMORY_TAG) override;
    void* ZeroMemoryReiTo(void*, u64) override;
    void* CopyMemoryImpl(void*, const void*, u64) override;
    void* SetMemoryImpl(void*, u8, u64) override;
private:

    void* allocatePoolCall(uint32_t, uint32_t);
    void deAllocatePoolCall(void*, uint32_t);

    void* allocateSystemCall(uint32_t, uint32_t);
    void deAllocateSystemCall(void*, uint32_t);
    //Memory For Small Object, new and delete often
    RTBlock* HeadSmallTarget;
    RTBlock* TailSmallTarget;

    //Memory For Large Object, Not A Usually Changed Target
    RTBlock* HeadLargeTarget;
    RTBlock* TailLargeTarget;

    size_t TotalAllocatedMemory;
    size_t PeakAllocatedMemory;

    std::mutex MemoryMutex;
    void* pTrackedMemoryPoolBuffer;
    void* pUntrackedMemoryPoolBuffer;
    size_t staticMemoryCallStep = 1;

    static RTDebugStackMemoryManager _instance;
public:
    const static constexpr size_t defaultSmallTargetMemorySize = 64 * 1024;
    const static constexpr size_t defaultSmallTargetMemoryAlignment = alignof(std::max_align_t);

    const static constexpr size_t defaultLargeTargetMemorySysCallSize = 1024 * 1024;
};
}

#endif
