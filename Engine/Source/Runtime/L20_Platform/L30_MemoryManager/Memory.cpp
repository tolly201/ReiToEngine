#include "Memory.h"

#if defined(_MSC_VER)
extern "C" long _InterlockedExchange(volatile long* Target, long Value);
#endif

RT_FORCEINLINE void ReiToEngine::MemoryManager::rt_spin_lock(volatile u32* s) {
#if defined(__clang__) || defined(__GNUC__)
    while (__sync_lock_test_and_set(s, 1)) {
        // busy-wait
    }
#elif defined(_MSC_VER)
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


namespace ReiToEngine
{
    // static void rt_spin_lock(volatile u32* s);
    // static void rt_spin_unlock(volatile u32* s);
    MemoryManager::MemoryManager(): wrappers(nullptr), is_set(false), mm_count(0){};
    MemoryManager::~MemoryManager()
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

    b8 MemoryManager::IsSeted() const { return is_set; }
    b8 MemoryManager::IsValid() const { return mm_count > 0; }

    b8 MemoryManager::SetMemoryManager(void* MM, EMEMORY_MANAGER_TYPE t, b8 is_default)
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

    b8 MemoryManager::SetDefaultMemoryManager()
    {
        return SetMemoryManager(nullptr, EMEMORY_MANAGER_TYPE::STANDARDC, true);
    }

    void* MemoryManager::Allocate(u64 size, u8 alignment, RT_MEMORY_TAG tag)
    {
        RT_ASSERT_MESSAGE(wrappers, "MemoryManager not initialized in Allocate.");
        // Copy handle/type under lock to avoid tearing
    rt_spin_lock(&lock_state);
        void* handle = wrappers->handle;
        EMEMORY_MANAGER_TYPE type = wrappers->type;
    rt_spin_unlock(&lock_state);
        // Determine desired user alignment (power-of-two rounding for safety)
        u64 user_alignment = alignment == 0 ? RT_SYSTEM_MAX_ALLIGNMENT : alignment;
        if (user_alignment < RT_SYSTEM_MAX_ALLIGNMENT) user_alignment = RT_SYSTEM_MAX_ALLIGNMENT;
        if (user_alignment & (user_alignment - 1)) { u64 p = 1; while (p < user_alignment) p <<= 1; user_alignment = p; }

        // Fast path: alignment request does not exceed system max -> we can use a fixed padded header
        constexpr u64 fixed_head_padded = (sizeof(MemoryManagerWrapperLayerHeadInfo) + RT_SYSTEM_MAX_ALLIGNMENT - 1) & ~(RT_SYSTEM_MAX_ALLIGNMENT - 1);
        bool use_fixed = user_alignment <= RT_SYSTEM_MAX_ALLIGNMENT;

        u64 request_size = 0;
        if (use_fixed) {
            // We just need header (padded) + user size
            request_size = fixed_head_padded + size;
        } else {
            // Fallback: need slack space for larger alignment
            request_size = size + head_info_size + user_alignment;
        }
        void* raw = nullptr;
        switch (type)
        {
            case EMEMORY_MANAGER_TYPE::MIMALLOC:
                raw = static_cast<RTMimallocManager*>(handle)->Allocate(request_size, (u8)RT_SYSTEM_MAX_ALLIGNMENT, tag); break;
            case EMEMORY_MANAGER_TYPE::STANDARDC:
                raw = static_cast<RTCMemoryManager*>(handle)->Allocate(request_size, (u8)RT_SYSTEM_MAX_ALLIGNMENT, tag); break;
            case EMEMORY_MANAGER_TYPE::BINNED:
                raw = static_cast<BinnedMemoryManager*>(handle)->Allocate(request_size, (u8)RT_SYSTEM_MAX_ALLIGNMENT, tag); break;
            case EMEMORY_MANAGER_TYPE::CUSTOM:
                raw = static_cast<ICustomMemoryManager*>(handle)->Allocate(request_size, (u8)RT_SYSTEM_MAX_ALLIGNMENT, tag); break;
            default:
                RT_ASSERT_MESSAGE(false, "Unknown memory manager type in MemoryManager::Allocate.");
                return nullptr;
        }
        RT_ASSERT_MESSAGE(raw != nullptr, "Memory allocation failed in MemoryManager::Allocate.");
        uintptr_t raw_addr = reinterpret_cast<uintptr_t>(raw);
        if (use_fixed) {
            // raw is assumed system aligned -> raw + fixed_head_padded is also system aligned, satisfying user_alignment
            uintptr_t user_addr = raw_addr + fixed_head_padded;
            uintptr_t header_addr = user_addr - head_info_size; // header sits just before user data inside padded region
            RT_ASSERT_MESSAGE((user_addr % user_alignment) == 0, "Fixed-path alignment failure (unexpected).\n");
            MemoryManagerWrapperLayerHeadInfo* head = reinterpret_cast<MemoryManagerWrapperLayerHeadInfo*>(header_addr);
            head->allocator = handle;
            head->actual_offset = (u64)(header_addr - raw_addr);
            head->actual_size = request_size;
            void* user_ptr = reinterpret_cast<void*>(user_addr);
            RT_LOG_WARN("Allocated memory (fixed path) from allocator: {}", (void*)handle);
            return user_ptr;
        } else {
            // Dynamic path for larger-than-system alignment
            uintptr_t search_begin = raw_addr + head_info_size;
            uintptr_t aligned_candidate = (search_begin + (user_alignment - 1)) & ~(user_alignment - 1);
            uintptr_t region_end = raw_addr + request_size;
            while (aligned_candidate + size > region_end) {
                uintptr_t next = aligned_candidate + user_alignment;
                if (next <= aligned_candidate || next + size > region_end) {
                    RT_ASSERT_MESSAGE(false, "Failed to place aligned allocation with header inside reserved block (logic error - dynamic).");
                    return nullptr;
                }
                aligned_candidate = next;
            }
            uintptr_t header_addr = aligned_candidate - head_info_size;
            RT_ASSERT_MESSAGE(header_addr >= raw_addr && header_addr + head_info_size <= region_end, "Header out of reserved range (dynamic).");
            MemoryManagerWrapperLayerHeadInfo* head = reinterpret_cast<MemoryManagerWrapperLayerHeadInfo*>(header_addr);
            head->allocator = handle;
            head->actual_offset = (u64)(header_addr - raw_addr);
            head->actual_size = request_size;
            void* user_ptr = reinterpret_cast<void*>(aligned_candidate);
            RT_ASSERT_MESSAGE(((uintptr_t)user_ptr % user_alignment) == 0, "User pointer is not properly aligned after allocation (dynamic path).");
            RT_LOG_WARN("Allocated memory (dynamic path) from allocator: {}", (void*)handle);
            return user_ptr;
        }
    }
    void MemoryManager::Free(void* addr, [[maybe_unused]]u64 size, RT_MEMORY_TAG tag)
    {
        if (!addr) return;
    RT_ASSERT_MESSAGE(wrappers, "MemoryManager not initialized in Free.");
        // Header is stored immediately before user region
        MemoryManagerWrapperLayerHeadInfo* head = reinterpret_cast<MemoryManagerWrapperLayerHeadInfo*>(static_cast<u8*>(addr) - head_info_size);
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

        u64 total_size = head->actual_size; // total size requested when allocated
        switch (type)
        {
            case EMEMORY_MANAGER_TYPE::MIMALLOC:
                return static_cast<RTMimallocManager*>(handle)->Free((void*)((uintptr_t)head - head->actual_offset), total_size, tag);
            case EMEMORY_MANAGER_TYPE::STANDARDC:
                return static_cast<RTCMemoryManager*>(handle)->Free((void*)((uintptr_t)head - head->actual_offset), total_size, tag);
            case EMEMORY_MANAGER_TYPE::BINNED:
                return static_cast<BinnedMemoryManager*>(handle)->Free((void*)((uintptr_t)head - head->actual_offset), total_size, tag);
            case EMEMORY_MANAGER_TYPE::CUSTOM:
                return static_cast<ICustomMemoryManager*>(handle)->Free((void*)((uintptr_t)head - head->actual_offset), total_size, tag);
            default:
                RT_LOG_ERROR("Unknown memory manager type in MemoryManager::Free. Falling back to Mimalloc.");
                return static_cast<RTMimallocManager*>(handle)->Free((void*)((uintptr_t)head - head->actual_offset), total_size, tag);
        }
    }
    void* MemoryManager::ZeroMemoryReiTo(void* addr, u64 size)
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
    void* MemoryManager::CopyMemoryReiTo(void* dest, const void* src, u64 size)
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
    void* MemoryManager::SetMemory(void* addr, u8 value, u64 size)
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
    char* MemoryManager::GetMemoryUsageReport(){
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
}
