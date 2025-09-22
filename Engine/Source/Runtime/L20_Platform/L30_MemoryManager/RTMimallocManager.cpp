#include "RTMimallocManager.h"
#include "mimalloc.h"
namespace ReiToEngine
{
RTMimallocManager::RTMimallocManager()
{
    // 可选：初始化线程本地状态（通常不必手动调用）
    // mi_thread_init();
}

RTMimallocManager::~RTMimallocManager()
{
    // 可选：强制回收线程缓存，便于测试或进程退出时更干净
    mi_collect(true);
    // 可选：打印统计（仅在需要时）
    // mi_stats_print(nullptr);
}

void* RTMimallocManager::AllocateImpl(u64 size, u8 align, RT_MEMORY_TAG /*tag*/)
{
    if (size == 0) size = 1; // 与多数分配器一致，对 0 给出最小分配
    if (align == 0 || align <= alignof(std::max_align_t))
        return mi_malloc(static_cast<size_t>(size));
    // 指定对齐分配
    return mi_malloc_aligned(static_cast<size_t>(size), static_cast<size_t>(align));
}

void RTMimallocManager::FreeImpl(void* p, u64 /*size*/, RT_MEMORY_TAG /*tag*/)
{
    if (p) mi_free(p);
}

void* RTMimallocManager::ZeroMemoryImpl(void* dst, u64 size)
{
    if (dst && size > 0) {
        RT_Platform_SYSZeroMemory(dst, size);
    }
    return dst;
}

void* RTMimallocManager::CopyMemoryImpl(void* dst, const void* src, u64 size)
{
    if (dst && src && size > 0) {
        RT_Platform_SYSCopyMemory(dst, src, size);
    }
    return dst;
}

void* RTMimallocManager::SetMemoryImpl(void* dst, u8 value, u64 size)
{
    if (dst && size > 0) {
        RT_Platform_SYSSetMemory(dst, value, size);
    }
    return dst;
}
} // namespace ReiToEngine
