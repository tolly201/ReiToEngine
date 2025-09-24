// 精简：只保留当前计划继续维护的内存管理器测试 (mimalloc 别名实现、Binned、RTC)。
// 其它 (DebugHeap/DebugStack/ReleaseHeap 等) 按用户指示将废弃，不纳入测试，避免无意义构建失败。
#include "../Core/TestCore.h"
#include "L20_Platform/L30_MemoryManager/RTMimallocManager.h"
#include "L20_Platform/L30_MemoryManager/BinnedMemoryManager.h"
#include "L20_Platform/L30_MemoryManager/RTCMemoryManager.h"

using namespace ReiToEngine;

// Minimal allocation pattern for active managers: allocate -> fill -> verify -> free
template <typename Manager>
static void basic_alloc_pattern(){
    Manager mgr;
    constexpr u64 Size = 128;
    void* p = mgr.Allocate(Size, 16, RT_MEMORY_TAG::APPLICATION);
    RT_EXPECT_TRUE(p != nullptr);
    unsigned char* b = static_cast<unsigned char*>(p);
    for(u64 i=0;i<Size;++i) b[i] = static_cast<unsigned char>(i & 0xFF);
    bool ok = true; for(u64 i=0;i<Size;++i){ if(b[i] != (i & 0xFF)){ ok=false; break; }}
    RT_EXPECT_TRUE(ok);
    mgr.Free(p, Size, RT_MEMORY_TAG::APPLICATION);
    RT_LOG_INFO(mgr.GetMemoryUsageReport());
}

RT_TEST_ST("Memory", Mgr_Mimalloc_Basic, "memory","mm-impl"){ basic_alloc_pattern<RTMimallocManager>(); }
RT_TEST_ST("Memory", Mgr_Binned_Basic,   "memory","mm-impl","binned"){ basic_alloc_pattern<BinnedMemoryManager>(); }
RT_TEST_ST("Memory", Mgr_RTC_Basic,      "memory","mm-impl"){ basic_alloc_pattern<RTCMemoryManager>(); }
