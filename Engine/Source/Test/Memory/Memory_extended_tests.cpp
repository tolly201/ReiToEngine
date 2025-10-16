#include "../Core/TestCore.h"
#include "L20_Platform/L23_MemoryManager/RTMimallocManager.h"
#include "L20_Platform/L23_MemoryManager/BinnedMemoryManager.h"
#include "L20_Platform/L23_MemoryManager/RTCMemoryManager.h"
#include <vector>
#include <cstdint>
#include <cstring>

using namespace ReiToEngine;

// 仅针对仍保留三种管理器：Mimalloc / Binned / RTC
// 辅助：对齐检查
static bool is_aligned(void* p, size_t alignment){
    return (reinterpret_cast<uintptr_t>(p) & (alignment-1)) == 0;
}

template <typename Manager>
static void alloc_alignment_sweep(){
    Manager mgr;
    const u64 size = 64;
    const u8 aligns[] = {1,2,4,8,16,32,64};
    for(u8 a: aligns){
        void* p = mgr.Allocate(size, a, RT_MEMORY_TAG::APPLICATION);
        RT_EXPECT_TRUE(p != nullptr);
        RT_EXPECT_TRUE(is_aligned(p, a==1?1:a));
        mgr.Free(p, size, RT_MEMORY_TAG::APPLICATION);
    }
}

template <typename Manager>
static void size_gradient_bins(){
    Manager mgr;
    // 尤其对 Binned：测试跨越边界 15->16, 16->17, 31->32, 63->64, 127->128, 255->256, 511->512, 1023->1024, 2047->2048
    const u64 sizes[] = {15,16,17,31,32,63,64,127,128,255,256,511,512,1023,1024,1500,2047,2048,4096};
    for(u64 s: sizes){
        void* p = mgr.Allocate(s, 16, RT_MEMORY_TAG::APPLICATION);
        RT_EXPECT_TRUE(p != nullptr);
        // 简单写入校验
        unsigned char* b = static_cast<unsigned char*>(p);
        for(u64 i=0;i<s;++i){ b[i] = static_cast<unsigned char>(i & 0xFF); }
        for(u64 i=0;i<s;++i){ if(b[i] != (i & 0xFF)){ RT_EXPECT_TRUE(false); break; } }
        mgr.Free(p, s, RT_MEMORY_TAG::APPLICATION);
    }
}

template <typename Manager>
static void zero_copy_set_cycle(){
    Manager mgr;
    constexpr u64 N = 256;
    void* p = mgr.Allocate(N, 32, RT_MEMORY_TAG::APPLICATION);
    RT_EXPECT_TRUE(p != nullptr);
    // Zero
    mgr.ZeroMemoryReiTo(p, N);
    unsigned char* b = static_cast<unsigned char*>(p);
    bool allZero = true; for(u64 i=0;i<N;++i){ if(b[i] != 0){ allZero=false; break; } }
    RT_EXPECT_TRUE(allZero);
    // Set pattern 0x7F
    mgr.SetMemory(p, 0x7F, N);
    bool allPattern = true; for(u64 i=0;i<N;++i){ if(b[i] != 0x7F){ allPattern=false; break; } }
    RT_EXPECT_TRUE(allPattern);
    // Copy to another block
    void* q = mgr.Allocate(N, 32, RT_MEMORY_TAG::APPLICATION);
    RT_EXPECT_TRUE(q != nullptr);
    mgr.CopyMemoryReiTo(q, p, N);
    unsigned char* qb = static_cast<unsigned char*>(q);
    bool copyOk = true; for(u64 i=0;i<N;++i){ if(qb[i] != 0x7F){ copyOk=false; break; } }
    RT_EXPECT_TRUE(copyOk);
    mgr.Free(q, N, RT_MEMORY_TAG::APPLICATION);
    mgr.Free(p, N, RT_MEMORY_TAG::APPLICATION);
}

template <typename Manager>
static void repeated_alloc_free_pattern(){
    Manager mgr;
    constexpr int loops = 64;
    constexpr u64 sz = 128;
    for(int i=0;i<loops;++i){
        void* p = mgr.Allocate(sz, 16, RT_MEMORY_TAG::APPLICATION);
        RT_EXPECT_TRUE(p != nullptr);
        mgr.Free(p, sz, RT_MEMORY_TAG::APPLICATION);
    }
}

// Mimalloc
RT_TEST_ST("Memory", Mimalloc_alignment_sweep, "memory","memory-advanced","mm-impl"){ alloc_alignment_sweep<RTMimallocManager>(); }
RT_TEST_ST("Memory", Mimalloc_size_gradient,   "memory","memory-advanced","mm-impl"){ size_gradient_bins<RTMimallocManager>(); RT_LOG_INFO_PLATFORM("Mimalloc size gradient done"); }
RT_TEST_ST("Memory", Mimalloc_zero_copy_set,   "memory","memory-advanced","mm-impl"){ zero_copy_set_cycle<RTMimallocManager>(); RT_LOG_INFO_PLATFORM("Mimalloc zero/copy/set done"); }
RT_TEST_ST("Memory", Mimalloc_repeated_alloc_free, "memory","memory-advanced","mm-impl"){ repeated_alloc_free_pattern<RTMimallocManager>(); RT_LOG_INFO_PLATFORM("Mimalloc repeated alloc/free done"); }

// Binned
RT_TEST_ST("Memory", Binned_alignment_sweep, "memory","memory-advanced","mm-impl","binned"){ alloc_alignment_sweep<BinnedMemoryManager>(); }
RT_TEST_ST("Memory", Binned_size_gradient,   "memory","memory-advanced","mm-impl","binned"){ size_gradient_bins<BinnedMemoryManager>(); RT_LOG_INFO_PLATFORM("Binned size gradient done"); }
RT_TEST_ST("Memory", Binned_zero_copy_set,   "memory","memory-advanced","mm-impl","binned"){ zero_copy_set_cycle<BinnedMemoryManager>(); RT_LOG_INFO_PLATFORM("Binned zero/copy/set done"); }
RT_TEST_ST("Memory", Binned_repeated_alloc_free, "memory","memory-advanced","mm-impl","binned"){ repeated_alloc_free_pattern<BinnedMemoryManager>(); RT_LOG_INFO_PLATFORM("Binned repeated alloc/free done"); }

// RTC
RT_TEST_ST("Memory", RTC_alignment_sweep, "memory","memory-advanced","mm-impl"){ alloc_alignment_sweep<RTCMemoryManager>(); }
RT_TEST_ST("Memory", RTC_size_gradient,   "memory","memory-advanced","mm-impl"){ size_gradient_bins<RTCMemoryManager>(); RT_LOG_INFO_PLATFORM("RTC size gradient done"); }
RT_TEST_ST("Memory", RTC_zero_copy_set,   "memory","memory-advanced","mm-impl"){ zero_copy_set_cycle<RTCMemoryManager>(); RT_LOG_INFO_PLATFORM("RTC zero/copy/set done"); }
RT_TEST_ST("Memory", RTC_repeated_alloc_free, "memory","memory-advanced","mm-impl"){ repeated_alloc_free_pattern<RTCMemoryManager>(); RT_LOG_INFO_PLATFORM("RTC repeated alloc/free done"); }
