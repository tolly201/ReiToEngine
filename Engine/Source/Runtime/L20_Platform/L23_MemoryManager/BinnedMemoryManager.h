#ifndef PLATFORM_BINNED_MEMORY_MANAGER_H
#define PLATFORM_BINNED_MEMORY_MANAGER_H

#include "IMemoryManager.h"
#include <cstdint>

namespace ReiToEngine {

// 一个面向 ECS 的分箱（Binned）分配器：
// - 小块（<= MaxBinSize）使用固定大小块的页式池分配，降低碎片、提升吞吐
// - 大块或超对齐需求回退到 mimalloc（对齐版本）
class RTENGINE_API BinnedMemoryManager : public RTMemoryManager<BinnedMemoryManager> {
    friend class RTMemoryManager<BinnedMemoryManager>;

public:
    BinnedMemoryManager();
    ~BinnedMemoryManager();

protected:
    void* AllocateImpl(u64 size, u8 alignment, RT_MEMORY_TAG tag) override;
    void  FreeImpl(void* addr, u64 size, RT_MEMORY_TAG tag) override;
    void* ZeroMemoryImpl(void* addr, u64 size) override;
    void* CopyMemoryImpl(void* dst, const void* src, u64 size) override;
    void* SetMemoryImpl(void* dst, u8 value, u64 size) override;

private:
    struct FreeNode { FreeNode* next; };
    struct Page {
        void*   rawBase;     // 原始系统分配基址（用于释放）
        void*   alignedBase; // 第一个块的起始地址（按块大小对齐）
        u64     rawSize;     // 系统实际分配的总字节数
        Page*   next;
    };

    struct Bin {
        u32      blockSize;  // 每个块的大小（power-of-two）
        FreeNode* freeList;  // 空闲块单链表
        Page*     pages;     // 该 bin 的页链表（用于析构时统一释放）
    };

private:
    static constexpr u32 MinBlockSize = 16u;     // 最小块 16B
    static constexpr u32 MaxBlockSize = 2048u;   // 最大块 2KB（大于则回退到系统）
    static constexpr u32 PageSize     = 64u * 1024u; // 每页 64KB

    // Bin 列表：16,32,64,128,256,512,1024,2048
    static constexpr u32 BinCount = 8u;
    Bin Bins[BinCount] = {};

private:
    // 工具函数
    static inline bool IsPowerOfTwo(u32 x) { return (x & (x - 1)) == 0; }
    static inline u32  NextPowerOfTwo(u32 x) {
        if (x <= 1) return 1;
        --x;
        x |= x >> 1; x |= x >> 2; x |= x >> 4; x |= x >> 8; x |= x >> 16;
        return x + 1;
    }
    static inline std::uintptr_t AlignUp(std::uintptr_t p, u32 align) {
        const std::uintptr_t mask = static_cast<std::uintptr_t>(align - 1);
        return (p + mask) & ~mask;
    }

    int  BinIndexFor(u32 size) const;         // 根据 size 求 bin 下标（-1 表示用系统）
    void* AllocateFromBin(Bin& bin, u32 blockSize, u8 alignment);
    void  PushFree(Bin& bin, void* p);
};

} // namespace ReiToEngine

#endif // PLATFORM_BINNED_MEMORY_MANAGER_H
