#include "../Core/TestCore.h"
#include "L20_Platform/L30_MemoryManager/Memory.h"
#include "L20_Platform/L31_SingletonFactory/SingletonFactory.h"
#include <cstring>

using namespace ReiToEngine;

// 基础：分配/写入/释放，总体统计是否变化（只做非零与回滚检查，避免依赖具体实现的细粒度行为）
RT_TEST_ST("Memory", BasicAllocFree, "memory","alloc"){
    constexpr u64 Size = 256;
    auto& mm = GetMemoryManager();
    // 记录初始（只能间接通过报告字符串，而当前没有直接 getter；因此只做功能验证：写入成功 & 释放后不崩）
    void* p = mm.Allocate(Size, 8, RT_MEMORY_TAG::APPLICATION);
    RT_EXPECT_TRUE(p != nullptr);
    // ZeroMemoryImpl 会清零，此处验证全部为 0
    u8* bytes = static_cast<u8*>(p);
    bool allZero = true;
    for(u64 i=0;i<Size;++i){ if(bytes[i] != 0){ allZero=false; break; } }
    RT_EXPECT_TRUE(allZero);
    // 写入后复制另一块再比对
    void* q = mm.Allocate(Size, 8, RT_MEMORY_TAG::APPLICATION);
    RT_EXPECT_TRUE(q != nullptr);
    mm.SetMemory(q, 0xAB, Size);
    mm.CopyMemory(p, q, Size);
    bool copied = true;
    for(u64 i=0;i<Size;++i){ if(bytes[i] != 0xAB){ copied=false; break; } }
    RT_EXPECT_TRUE(copied);
    mm.Free(q, Size, RT_MEMORY_TAG::APPLICATION);
    mm.Free(p, Size, RT_MEMORY_TAG::APPLICATION);
    const char* report = mm.GetMemoryUsageReport();
    RT_LOG_INFO(report);
}

// 对齐：请求较高对齐并检测指针地址满足对齐
RT_TEST_ST("Memory", AlignmentAlloc, "memory","alignment"){
    auto& mm = GetMemoryManager();
    constexpr u64 Size = 64;
    constexpr u8 Align = 64; // 比 max_align_t 更高
    void* p = mm.Allocate(Size, Align, RT_MEMORY_TAG::APPLICATION);
    RT_EXPECT_TRUE(p != nullptr);
    RT_EXPECT_TRUE(reinterpret_cast<uintptr_t>(p) % Align == 0);
    mm.Free(p, Size, RT_MEMORY_TAG::APPLICATION);
    RT_LOG_INFO(mm.GetMemoryUsageReport());
}

// 批量：多次分配再释放，保证全部非空；不做性能断言，仅健壮性
RT_TEST_ST("Memory", BulkAlloc, "memory","alloc","stress"){
    auto& mm = GetMemoryManager();
    constexpr int N = 128;
    void* blocks[N];
    for(int i=0;i<N;++i){
        blocks[i] = mm.Allocate(32 + i, 16, RT_MEMORY_TAG::APPLICATION);
        RT_EXPECT_TRUE(blocks[i] != nullptr);
    }
    for(int i=0;i<N;++i){
        mm.Free(blocks[i], 32 + i, RT_MEMORY_TAG::APPLICATION);
    }
    RT_LOG_INFO(mm.GetMemoryUsageReport());
}

// 大块分配：超出典型小块/中等分箱阈值（例如 1MB+），验证成功并可写入/释放
RT_TEST_ST("Memory", LargeAllocationFallback, "memory","alloc","large"){
    auto& mm = GetMemoryManager();
    constexpr u64 Size = 1 * 1024 * 1024 + 13; // 1MB+13 形成非整齐大小
    void* p = mm.Allocate(Size, 16, RT_MEMORY_TAG::APPLICATION);
    RT_EXPECT_TRUE(p != nullptr);
    // 写入首尾保持可访问
    u8* bytes = static_cast<u8*>(p);
    bytes[0]=0xCD; bytes[Size-1]=0xEF;
    RT_EXPECT_TRUE(bytes[0]==0xCD && bytes[Size-1]==0xEF);
    mm.Free(p, Size, RT_MEMORY_TAG::APPLICATION);
    RT_LOG_INFO(mm.GetMemoryUsageReport());
}
