#include "BinnedMemoryManager.h"
#include "RTMimallocManager.h" // 用于大块或复杂对齐回退
#include "L20_Platform/L20_Memory/Include.h"
#include "mimalloc.h"

namespace ReiToEngine {

BinnedMemoryManager::BinnedMemoryManager() {
    // 初始化 bin 尺寸：16,32,64,128,256,512,1024,2048
    static constexpr u32 sizes[BinCount] = {16,32,64,128,256,512,1024,2048};
    for (u32 i = 0; i < BinCount; ++i) {
        Bins[i].blockSize = sizes[i];
        Bins[i].freeList = nullptr;
        Bins[i].pages = nullptr;
    }
}

BinnedMemoryManager::~BinnedMemoryManager() {
    // 释放所有页（bin 内部的空闲块无需单独处理）
    for (u32 i = 0; i < BinCount; ++i) {
        Page* p = Bins[i].pages;
        while (p) {
            Page* nxt = p->next;
            mi_free(p->rawBase);
            p = nxt;
        }
        Bins[i].pages = nullptr;
        Bins[i].freeList = nullptr;
    }
}

int BinnedMemoryManager::BinIndexFor(u32 size) const {
    if (size == 0) size = 1;
    if (size > MaxBlockSize) return -1;
    u32 bs = NextPowerOfTwo(size);
    if (bs < MinBlockSize) bs = MinBlockSize;
    // 映射到 bins
    for (u32 i = 0; i < BinCount; ++i) {
        if (Bins[i].blockSize == bs) return static_cast<int>(i);
    }
    return -1;
}

void* BinnedMemoryManager::AllocateFromBin(Bin& bin, u32 blockSize, u8 alignment) {
    // 如果有空闲块，直接弹出
    if (bin.freeList) {
        FreeNode* node = bin.freeList;
        bin.freeList = node->next;
        return static_cast<void*>(node);
    }

    // 否则新分配一页，并切分为多个块挂到 freeList
    size_t rawBytes = PageSize + alignment + sizeof(Page);
    void* raw = mi_malloc(rawBytes);
    if (!raw) return nullptr;

    // Page header 我们放在单独的 mi_malloc 上下文之外，为简化，
    // 这里将 Page 结构放在独立的堆对象上。
    Page* pageHdr = reinterpret_cast<Page*>(mi_malloc(sizeof(Page)));
    if (!pageHdr) {
        mi_free(raw);
        return nullptr;
    }
    pageHdr->rawBase = raw;
    pageHdr->rawSize = rawBytes;

    // 计算对齐后的起点
    std::uintptr_t base = reinterpret_cast<std::uintptr_t>(raw);
    std::uintptr_t aligned = AlignUp(base, blockSize);
    pageHdr->alignedBase = reinterpret_cast<void*>(aligned);

    // 将页插入到 bin 的页链表头
    pageHdr->next = bin.pages;
    bin.pages = pageHdr;

    // 将页切分成链表
    u32 capacity = static_cast<u32>((PageSize - (aligned - base)) / blockSize);
    char* cursor = reinterpret_cast<char*>(aligned);
    FreeNode* head = nullptr;
    for (u32 i = 0; i < capacity; ++i) {
        auto* node = reinterpret_cast<FreeNode*>(cursor);
        node->next = head;
        head = node;
        cursor += blockSize;
    }
    bin.freeList = head->next; // 预先弹出一个作为返回块
    return static_cast<void*>(head);
}

void* BinnedMemoryManager::AllocateImpl(u64 size64, u8 alignment, RT_MEMORY_TAG /*tag*/) {
    u32 size = static_cast<u32>(size64 > 0 ? size64 : 1);

    // 对齐如果大于块大小或者不是2的幂，回退到 mimalloc 对齐分配
    if (alignment && !IsPowerOfTwo(alignment)) alignment = 0; // 防御：非2幂视为未指定

    int idx = BinIndexFor(size);
    if (idx < 0) {
        // 大块或超范围，回退 mimalloc
        if (alignment && alignment > alignof(std::max_align_t))
            return mi_malloc_aligned(size, alignment);
        return mi_malloc(size);
    }

    Bin& bin = Bins[idx];
    const u32 blockSize = bin.blockSize;
    // 如果对齐大于 blockSize，则无法满足，回退 mimalloc 对齐分配
    if (alignment && alignment > blockSize) {
        return mi_malloc_aligned(size, alignment);
    }

    return AllocateFromBin(bin, blockSize, alignment ? alignment : blockSize);
}

void BinnedMemoryManager::PushFree(Bin& bin, void* p) {
    auto* node = reinterpret_cast<FreeNode*>(p);
    node->next = bin.freeList;
    bin.freeList = node;
}

void BinnedMemoryManager::FreeImpl(void* addr, u64 size64, RT_MEMORY_TAG /*tag*/) {
    if (!addr) return;
    u32 size = static_cast<u32>(size64 > 0 ? size64 : 1);
    int idx = BinIndexFor(size);
    if (idx < 0) {
        mi_free(addr);
        return;
    }
    Bin& bin = Bins[idx];
    // 直接归还到 bin 的 freeList
    PushFree(bin, addr);
}

void* BinnedMemoryManager::ZeroMemoryImpl(void* dst, u64 size) {
    if (dst && size > 0) {
        RT_Platform_SYSZeroMemory(dst, size);
    }
    return dst;
}

void* BinnedMemoryManager::CopyMemoryImpl(void* dst, const void* src, u64 size) {
    if (dst && src && size > 0) {
        RT_Platform_SYSCopyMemory(dst, src, size);
    }
    return dst;
}

void* BinnedMemoryManager::SetMemoryImpl(void* dst, u8 value, u64 size) {
    if (dst && size > 0) {
        RT_Platform_SYSSetMemory(dst, value, size);
    }
    return dst;
}

} // namespace ReiToEngine
