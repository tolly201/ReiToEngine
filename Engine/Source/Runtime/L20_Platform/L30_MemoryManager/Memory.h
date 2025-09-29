#ifndef CORE_PLATFORM_MEMORY_MEMORY_H
#define CORE_PLATFORM_MEMORY_MEMORY_H
#include "RTMimallocManager.h"
#include "RTCMemoryManager.h"
#include "BinnedMemoryManager.h"
#include "IBlock.h"
#include "ICustomMemoryManager.h"
namespace ReiToEngine
{
enum class EMEMORY_MANAGER_TYPE
{
    DEFAULT = 0,
    STANDARDC,
    MIMALLOC,
    BINNED,
    CUSTOM,
};
class MemoryManager
{
private:
    // Forward declarations for spinlock helpers (defined after class)
    static void rt_spin_lock(volatile u32* s);
    static void rt_spin_unlock(volatile u32* s);
public:
    MemoryManager();
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&) = delete;
    MemoryManager& operator=(MemoryManager&&) = delete;
    ~MemoryManager();
    b8 IsSeted() const;
    b8 IsValid() const;

    b8 SetMemoryManager(void* MM, EMEMORY_MANAGER_TYPE t, b8 is_default = false);

    b8 SetDefaultMemoryManager();
    void* Allocate(u64 size, u8 alignment, RT_MEMORY_TAG tag);
    void Free(void* addr, u64 size, RT_MEMORY_TAG tag);
    void* ZeroMemoryReiTo(void* addr, u64 size);
    void* CopyMemoryReiTo(void* dest, const void* src, u64 size);
    void* SetMemory(void* addr, u8 value, u64 size);
    char* GetMemoryUsageReport();
    private:
    struct MemoryManagerWrapperLayerHeadInfo {
        void* allocator;      // allocator handle
        u64   actual_offset;  // offset from raw base to header (for debug/back-calc)
        u64   actual_size;    // total size requested from underlying allocator
    };
    // Raw header size
    constexpr static u64 head_info_size = sizeof(MemoryManagerWrapperLayerHeadInfo);
    // System alignment we want every user pointer to satisfy at minimum.
    // Using max_align_t to cover fundamental types (C/C++ standard guarantee).
    // head_padded_size removed: header is placed immediately before the aligned user pointer; we search for a valid placement.

    struct MemoryManagerWrapper{
        void* handle;
        EMEMORY_MANAGER_TYPE type;
        b8 by_pass; // if true, do not delete in destructor
        MemoryManagerWrapper* pNext;
    };
    MemoryManagerWrapper* wrappers = nullptr;
    b8 is_set = false;
    u8 mm_count = 0;
    volatile u32 lock_state = 0;
};
}
#endif
