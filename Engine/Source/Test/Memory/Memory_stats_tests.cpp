#include "../Core/TestCore.h"
#include "L20_Platform/L23_MemoryManager/Memory.h"
#include "L20_Platform/L23_SingletonFactory/SingletonFactory.h"
#include <string>
#include <cstring>

using namespace ReiToEngine;

// 解析报告字符串中某个标签行的数值（不关心单位，只要能找到行）
static float extract_amount_for_tag(const char* report, const char* tag){
    const char* p = std::strstr(report, tag);
    if(!p) return -1.f;
    const char* colon = std::strchr(p, ':');
    if(!colon) return -1.f;
    float value = -1.f;
    std::sscanf(colon+1, " %f", &value);
    return value;
}

RT_TEST_ST("Memory", StatsIncreaseAndRollback, "memory","stats"){
    auto& mm = GetMemoryManager();
    const char* before = mm.GetMemoryUsageReport();
    float beforeApp = extract_amount_for_tag(before, "APPLICATION");

    constexpr u64 Size = 1024 * 3; // 3KB
    void* p = mm.Allocate(Size, 8, RT_MEMORY_TAG::APPLICATION);
    RT_EXPECT_TRUE(p != nullptr);
    const char* mid = mm.GetMemoryUsageReport();
    float midApp = extract_amount_for_tag(mid, "APPLICATION");
    RT_EXPECT_TRUE(midApp >= beforeApp); // 增加或相等（如果单位切换导致显示差）

    mm.Free(p, Size, RT_MEMORY_TAG::APPLICATION);
    const char* after = mm.GetMemoryUsageReport();
    float afterApp = extract_amount_for_tag(after, "APPLICATION");
    RT_EXPECT_TRUE(afterApp <= midApp);
    RT_LOG_INFO_PLATFORM(after);
}

// 同时申请 APPLICATION 与 CONTAINER 各自独立增长并释放后回滚
RT_TEST_ST("Memory", MixedTagIndependentStats, "memory","stats","mixed"){
    auto& mm = GetMemoryManager();
    const char* before = mm.GetMemoryUsageReport();
    float beforeApp = extract_amount_for_tag(before, "APPLICATION");
    float beforeCont = extract_amount_for_tag(before, "CONTAINER");

    constexpr u64 SizeA = 4096; // 4KB app
    constexpr u64 SizeC = 2048; // 2KB container
    void* pa = mm.Allocate(SizeA, 8, RT_MEMORY_TAG::APPLICATION);
    void* pc = mm.Allocate(SizeC, 8, RT_MEMORY_TAG::CONTAINER);
    RT_EXPECT_TRUE(pa && pc);
    const char* mid = mm.GetMemoryUsageReport();
    float midApp = extract_amount_for_tag(mid, "APPLICATION");
    float midCont = extract_amount_for_tag(mid, "CONTAINER");
    RT_EXPECT_TRUE(midApp >= beforeApp);
    RT_EXPECT_TRUE(midCont >= beforeCont);

    // 释放容器内存先观察应用标签不回滚
    mm.Free(pc, SizeC, RT_MEMORY_TAG::CONTAINER);
    const char* afterC = mm.GetMemoryUsageReport();
    float afterCont = extract_amount_for_tag(afterC, "CONTAINER");
    float afterAppStill = extract_amount_for_tag(afterC, "APPLICATION");
    RT_EXPECT_TRUE(afterCont <= midCont);
    RT_EXPECT_TRUE(afterAppStill == midApp); // 应用标签未释放保持

    mm.Free(pa, SizeA, RT_MEMORY_TAG::APPLICATION);
    const char* finalRep = mm.GetMemoryUsageReport();
    float finalApp = extract_amount_for_tag(finalRep, "APPLICATION");
    RT_EXPECT_TRUE(finalApp <= afterAppStill);
    RT_LOG_INFO_PLATFORM(finalRep);
}
