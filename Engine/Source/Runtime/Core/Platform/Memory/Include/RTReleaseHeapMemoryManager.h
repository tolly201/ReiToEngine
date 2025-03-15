#ifndef CORE_HAL_MEMORY_RTRELEASEHEAPMEMORYALLOCATOR_H
#define CORE_HAL_MEMORY_RTRELEASEHEAPMEMORYALLOCATOR_H
#include "IBlock.h"
#include "IMemoryManager.h"
#include <mutex>
#include <cstddef> // for std::max_align_t
#include "Core/HAL/Memory/Memory.h"
#include "RTMemoryPool.h"
namespace ReiToEngine
{
    class RTENGINE_API RTReleaseHeapMemoryManager : public RTMemoryManager<RTReleaseHeapMemoryManager>
    {
        friend class RTMemoryManager<RTReleaseHeapMemoryManager>;

    public:
        RTReleaseHeapMemoryManager();
        ~RTReleaseHeapMemoryManager();
        void *Allocate(size_t uiSize, size_t uiAlignment, bool bIsArray);
        void Deallocate(char *pcAddr, size_t uiAlignment, bool bIsArray);

    protected:
        void* allocateImpl(size_t, uint8_t, bool) override;
        void deallocateImpl(void*, uint8_t, bool) override;
    private:
        // Counts.
        enum
        {
            POOL_COUNT = 42
        };
        enum
        {
            POOL_MAX = 32768 + 1
        };

        FPoolTable PoolTable[POOL_COUNT], OsTable;
        FPoolInfo *PoolIndirect[32];
        FPoolTable *MemSizeToPoolTable[POOL_MAX];
        uint32_t SystemPageSize;
        std::mutex MemoryMutex;
        FPoolInfo *CreateIndirect()
        {
            FPoolInfo *Indirect = (FPoolInfo *)RTSysAlloc(2048 * sizeof(FPoolInfo));
            if (!Indirect)
            {
                return nullptr;
            }
            return Indirect;
        }
    };
}

#endif
