#ifndef CORE_HAL_MEMORY_RTRELEASEHEAPMEMORYALLOCATOR_H
#define CORE_HAL_MEMORY_RTRELEASEHEAPMEMORYALLOCATOR_H
#include "IBlock.h"
#include "IMemoryManager.h"
#include <mutex>
#include <cstddef> // for std::max_align_t
#include "Core/MinimalCore.h"
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
    void* AllocateImpl(u64, u8, RT_MEMORY_TAG) override;
    void FreeImpl(void*, u64, RT_MEMORY_TAG) override;
    void* ZeroMemoryImpl(void*, u64) override;
    void* CopyMemoryImpl(void*, const void*, u64) override;
    void* SetMemoryImpl(void*, u8, u64) override;
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
            FPoolInfo *Indirect = (FPoolInfo *)RT_HAL_SYSAlloc(2048 * sizeof(FPoolInfo));
            if (!Indirect)
            {
                return nullptr;
            }
            return Indirect;
        }
    };
}

#endif
