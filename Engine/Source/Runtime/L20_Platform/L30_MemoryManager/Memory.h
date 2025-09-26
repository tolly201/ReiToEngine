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
    MemoryManager(): wrappers(nullptr), is_set(false), mm_count(0){};
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&) = delete;
    MemoryManager& operator=(MemoryManager&&) = delete;
    ~MemoryManager()
    {
    // Acquire lock to avoid concurrent mutation during teardown
    rt_spin_lock(&lock_state);
        while(wrappers)
        {
            MemoryManagerWrapper* raw = wrappers;
            wrappers = wrappers->pNext;
            void* handle = raw->handle;
            EMEMORY_MANAGER_TYPE type = raw->type;
            b8 by_pass = raw->by_pass;
            RT_Platform_SYSFree(raw, sizeof(MemoryManagerWrapper));
            if (mm_count > 0) { --mm_count; }
            if (by_pass) continue; // Skip deletion if by_pass is true

            if (handle)
            {
                switch (type)
                {
                    case EMEMORY_MANAGER_TYPE::MIMALLOC:
                        delete static_cast<RTMimallocManager*>(handle);
                        break;
                    case EMEMORY_MANAGER_TYPE::STANDARDC:
                        delete static_cast<RTCMemoryManager*>(handle);
                        break;
                    case EMEMORY_MANAGER_TYPE::BINNED:
                        delete static_cast<BinnedMemoryManager*>(handle);
                        break;
                    case EMEMORY_MANAGER_TYPE::CUSTOM:
                        // Do not delete custom memory manager; user is responsible for its lifecycle
                        break;
                    default:
                        RT_LOG_WARN("Unknown memory manager type in MemoryManager destructor.");
                        break;
                }
                handle = nullptr;
            }
        }
        is_set = false;
        rt_spin_unlock(&lock_state);
    }

    b8 IsSeted() const { return is_set; }
    b8 IsValid() const { return mm_count > 0; }

    b8 SetMemoryManager(void* MM, EMEMORY_MANAGER_TYPE t, b8 is_default = false)
    {
        RT_ASSERT_MESSAGE(!is_set, "Memory manager can only be set once.");

    rt_spin_lock(&lock_state);
        MemoryManagerWrapper* newWrapper = static_cast<MemoryManagerWrapper*>( RT_Platform_SYSAlloc(sizeof(MemoryManagerWrapper)));

        is_set = !is_default;
        ++mm_count;
        newWrapper->pNext = wrappers;
        newWrapper->by_pass = false;
        wrappers = newWrapper;

        RT_LOG_WARN_FMT("Setting memory manager of type: {}", static_cast<void*>(newWrapper));

        //custom situation
        if (MM != nullptr)
        {
            newWrapper->handle = MM;
            newWrapper->by_pass = true;
            newWrapper->type = t;
            rt_spin_unlock(&lock_state);
            return true;
        }
        else
        {
            switch (t)
            {
                case EMEMORY_MANAGER_TYPE::MIMALLOC:
                    newWrapper->handle = new RTMimallocManager();
                    newWrapper->type = t;
                    break;
                case EMEMORY_MANAGER_TYPE::STANDARDC:
                    newWrapper->handle = new RTCMemoryManager();
                    newWrapper->type = t;
                    break;
                case EMEMORY_MANAGER_TYPE::BINNED:
                    newWrapper->handle = new BinnedMemoryManager();
                    newWrapper->type = t;
                    break;
                case EMEMORY_MANAGER_TYPE::CUSTOM:
                    RT_LOG_ERROR("CUSTOM backend requires external instance passed via MM (ICustomMemoryManager*).");
                    wrappers = newWrapper->pNext;
                    --mm_count;
                    is_set = false;
                    RT_Platform_SYSFree(newWrapper, sizeof(MemoryManagerWrapper));
                    rt_spin_unlock(&lock_state);
                    return false;
                default:
                    RT_LOG_ERROR("Unknown EMEMORY_MANAGER_TYPE specified in MemoryManager::SetMemoryManager. Fallback to Mimalloc.");
                    newWrapper->handle = new RTMimallocManager(); // Fallback to Mimalloc
                    newWrapper->type = EMEMORY_MANAGER_TYPE::MIMALLOC;
                    break;
            }
            rt_spin_unlock(&lock_state);


            RT_LOG_WARN_FMT("Create memory manager of type: {}", static_cast<void*>(newWrapper->handle));
            return true;
        }
        rt_spin_unlock(&lock_state);
        return false; // Should not reach here
    }

    b8 SetDefaultMemoryManager()
    {
        return SetMemoryManager(nullptr, EMEMORY_MANAGER_TYPE::STANDARDC, true);
    }

    void* Allocate(u64 size, u8 alignment, RT_MEMORY_TAG tag)
    {
        RT_ASSERT_MESSAGE(wrappers, "MemoryManager not initialized in Allocate.");
        // Copy handle/type under lock to avoid tearing
    rt_spin_lock(&lock_state);
        void* handle = wrappers->handle;
        EMEMORY_MANAGER_TYPE type = wrappers->type;
    rt_spin_unlock(&lock_state);

        void* mem;
        switch (type)
        {
            case EMEMORY_MANAGER_TYPE::MIMALLOC:
                mem = static_cast<RTMimallocManager*>(handle)->Allocate(size + head_info_size, alignment, tag);
                break;
            case EMEMORY_MANAGER_TYPE::STANDARDC:
                mem = static_cast<RTCMemoryManager*>(handle)->Allocate(size + head_info_size, alignment, tag);
                break;

            case EMEMORY_MANAGER_TYPE::BINNED:
                mem = static_cast<BinnedMemoryManager*>(handle)->Allocate(size + head_info_size, alignment, tag);
                break;
            case EMEMORY_MANAGER_TYPE::CUSTOM:
                mem = static_cast<ICustomMemoryManager*>(handle)->Allocate(size + head_info_size, alignment, tag);
                break;
            default:
                RT_ASSERT_MESSAGE(false, "Unknown memory manager type in MemoryManager::Allocate.");
                return nullptr;
        }

        RT_ASSERT_MESSAGE(mem != nullptr, "Memory allocation failed in MemoryManager::Allocate.");

        MemoryManagerWrapperLayerHeadInfo* head = static_cast<MemoryManagerWrapperLayerHeadInfo*>(mem);
        head->allocator = handle;

        RT_LOG_WARN("Allocated memory from allocator: {}", (void*)handle);

        mem = static_cast<void*>(static_cast<u8*>(mem) + head_info_size);
        return mem;
    }
    void Free(void* addr, u64 size, RT_MEMORY_TAG tag)
    {
        if (!addr) return;
    RT_ASSERT_MESSAGE(wrappers, "MemoryManager not initialized in Free.");
    rt_spin_lock(&lock_state);

    rt_spin_unlock(&lock_state);

    addr = static_cast<void*>(static_cast<u8*>(addr) - head_info_size);
        MemoryManagerWrapperLayerHeadInfo* head = static_cast<MemoryManagerWrapperLayerHeadInfo*>(addr);

        void* allocator = head->allocator;
        RT_LOG_WARN("Freeing memory allocated by allocator: {}", (void*)allocator);

        MemoryManagerWrapper* current = wrappers;
        b8 found = false;
        while (current)
        {
            RT_LOG_WARN("CHECK HANDLE: {}", (void*)current->handle);
            if (current->handle == allocator)
            {
                found = true;
                break;
            }
            current = current->pNext;
        }
        RT_ASSERT_MESSAGE(found, "Allocator not found in MemoryManager::Free. Possible double free or memory corruption.");
        void* handle = current->handle;
        EMEMORY_MANAGER_TYPE type = current->type;

        switch (type)
        {
            case EMEMORY_MANAGER_TYPE::MIMALLOC:
                return static_cast<RTMimallocManager*>(handle)->Free(addr, size + head_info_size, tag);
            case EMEMORY_MANAGER_TYPE::STANDARDC:
                return static_cast<RTCMemoryManager*>(handle)->Free(addr, size + head_info_size, tag);
            case EMEMORY_MANAGER_TYPE::BINNED:
                return static_cast<BinnedMemoryManager*>(handle)->Free(addr, size + head_info_size, tag);
            case EMEMORY_MANAGER_TYPE::CUSTOM:
                return static_cast<ICustomMemoryManager*>(handle)->Free(addr, size + head_info_size, tag);
            default:
                RT_LOG_ERROR("Unknown memory manager type in MemoryManager::Free. Falling back to Mimalloc.");
                return static_cast<RTMimallocManager*>(handle)->Free(addr, size + head_info_size, tag);
        }
    }
    void* ZeroMemoryReiTo(void* addr, u64 size)
    {
    RT_ASSERT_MESSAGE(wrappers, "MemoryManager not initialized in ZeroMemoryReiTo.");
    rt_spin_lock(&lock_state);
        void* handle = wrappers->handle;
        EMEMORY_MANAGER_TYPE type = wrappers->type;
    rt_spin_unlock(&lock_state);
        switch (type)
        {
            case EMEMORY_MANAGER_TYPE::MIMALLOC:
                return static_cast<RTMimallocManager*>(handle)->ZeroMemoryReiTo(addr, size);
            case EMEMORY_MANAGER_TYPE::STANDARDC:
                return static_cast<RTCMemoryManager*>(handle)->ZeroMemoryReiTo(addr, size);
            case EMEMORY_MANAGER_TYPE::BINNED:
                return static_cast<BinnedMemoryManager*>(handle)->ZeroMemoryReiTo(addr, size);
            case EMEMORY_MANAGER_TYPE::CUSTOM:
                return static_cast<ICustomMemoryManager*>(handle)->ZeroMemoryReiTo(addr, size);
            default:
                RT_LOG_ERROR("Unknown memory manager type in MemoryManager::ZeroMemoryReiTo. Falling back to Mimalloc.");
                return static_cast<RTMimallocManager*>(handle)->ZeroMemoryReiTo(addr, size);
        }
    }
    void* CopyMemoryReiTo(void* dest, const void* src, u64 size)
    {
    RT_ASSERT_MESSAGE(wrappers, "MemoryManager not initialized in CopyMemoryReiTo.");
    rt_spin_lock(&lock_state);
        void* handle = wrappers->handle;
        EMEMORY_MANAGER_TYPE type = wrappers->type;
    rt_spin_unlock(&lock_state);
        switch(type)
        {
            case EMEMORY_MANAGER_TYPE::MIMALLOC:
                return static_cast<RTMimallocManager*>(handle)->CopyMemoryReiTo(dest, src, size);
            case EMEMORY_MANAGER_TYPE::STANDARDC:
                return static_cast<RTCMemoryManager*>(handle)->CopyMemoryReiTo(dest, src, size);
            case EMEMORY_MANAGER_TYPE::BINNED:
                return static_cast<BinnedMemoryManager*>(handle)->CopyMemoryReiTo(dest, src, size);
            case EMEMORY_MANAGER_TYPE::CUSTOM:
                return static_cast<ICustomMemoryManager*>(handle)->CopyMemoryReiTo(dest, src, size);
            default:
                RT_LOG_ERROR("Unknown memory manager type in MemoryManager::CopyMemoryReiTo. Falling back to Mimalloc.");
                return static_cast<RTMimallocManager*>(handle)->CopyMemoryReiTo(dest, src, size);
        }
    }
    void* SetMemory(void* addr, u8 value, u64 size)
    {
    RT_ASSERT_MESSAGE(wrappers, "MemoryManager not initialized in SetMemory.");
    rt_spin_lock(&lock_state);
        void* handle = wrappers->handle;
        EMEMORY_MANAGER_TYPE type = wrappers->type;
    rt_spin_unlock(&lock_state);
        switch (type)
        {
            case EMEMORY_MANAGER_TYPE::MIMALLOC:
                return static_cast<RTMimallocManager*>(handle)->SetMemory(addr, value, size);
            case EMEMORY_MANAGER_TYPE::STANDARDC:
                return static_cast<RTCMemoryManager*>(handle)->SetMemory(addr, value, size);
            case EMEMORY_MANAGER_TYPE::BINNED:
                return static_cast<BinnedMemoryManager*>(handle)->SetMemory(addr, value, size);
            case EMEMORY_MANAGER_TYPE::CUSTOM:
                return static_cast<ICustomMemoryManager*>(handle)->SetMemory(addr, value, size);
            default:
                RT_LOG_ERROR("Unknown memory manager type in MemoryManager::SetMemory. Falling back to Mimalloc.");
                return static_cast<RTMimallocManager*>(handle)->SetMemory(addr, value, size);
        }
    }
    char* GetMemoryUsageReport(){
    RT_ASSERT_MESSAGE(wrappers, "MemoryManager not initialized in GetMemoryUsageReport.");
    rt_spin_lock(&lock_state);
        void* handle = wrappers->handle;
        EMEMORY_MANAGER_TYPE type = wrappers->type;
    rt_spin_unlock(&lock_state);
        switch (type)
        {
            case EMEMORY_MANAGER_TYPE::MIMALLOC:
                return static_cast<RTMimallocManager*>(handle)->GetMemoryUsageReport();
            case EMEMORY_MANAGER_TYPE::STANDARDC:
                return static_cast<RTCMemoryManager*>(handle)->GetMemoryUsageReport();
            case EMEMORY_MANAGER_TYPE::BINNED:
                return static_cast<BinnedMemoryManager*>(handle)->GetMemoryUsageReport();
            case EMEMORY_MANAGER_TYPE::CUSTOM:
                return static_cast<ICustomMemoryManager*>(handle)->GetMemoryUsageReport();
            default:
                RT_LOG_ERROR("Unknown memory manager type in MemoryManager::GetMemoryUsageReport. Falling back to Mimalloc.");
                return static_cast<RTMimallocManager*>(handle)->GetMemoryUsageReport();
        }
    }
private:
    struct MemoryManagerWrapperLayerHeadInfo {
        void* allocator;
    };

    constexpr static u64 head_info_size = RTAligned(sizeof(MemoryManagerWrapperLayerHeadInfo), 32);

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

// Out-of-class definitions for spinlock helpers
RT_FORCEINLINE void ReiToEngine::MemoryManager::rt_spin_lock(volatile u32* s) {
#if defined(__clang__) || defined(__GNUC__)
    while (__sync_lock_test_and_set(s, 1)) {
        // busy-wait
    }
#elif defined(_MSC_VER)
    extern "C" long _InterlockedExchange(volatile long* Target, long Value);
    while (_InterlockedExchange((volatile long*)s, 1) != 0) { /* spin */ }
#else
#   error "Unsupported compiler for spinlock"
#endif
}
RT_FORCEINLINE void ReiToEngine::MemoryManager::rt_spin_unlock(volatile u32* s) {
#if defined(__clang__) || defined(__GNUC__)
    __sync_lock_release(s);
#elif defined(_MSC_VER)
    *s = 0;
#endif
}
}
#endif
