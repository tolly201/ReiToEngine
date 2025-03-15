#include "../Include/RTDebugHeapMemoryManager.h"
#include "../Include/MemoryAllocator_cpp.h" // 引入 RTSysAlloc 和 RTSysFree
#include <iostream>
namespace ReiToEngine
{
RTDebugHeapMemoryManager::RTDebugHeapMemoryManager() :
    HeadSmallTarget(nullptr),
    TailSmallTarget(nullptr),
    TotalAllocatedMemory(0),
    PeakAllocatedMemory(0)
{
    // void* memory = allocateSystemCall(sizeof(RTDebugHeapMemoryManager), alignof(RTDebugHeapMemoryManager));
    // if (!memory)
    // {
    //     std::cerr << "RTDebugHeapMemoryManager: 初始化内存分配失败!" << std::endl;
    //     return;
    // }
    // std::cout << "RTDebugHeapMemoryManager: 初始化." << std::endl;
    // // 将分配的内存用于初始化单例实例
    // new (memory) RTDebugHeapMemoryManager();
}

RTDebugHeapMemoryManager::~RTDebugHeapMemoryManager()
{
    std::cout << "RTDebugHeapMemoryManager 析构函数" << std::endl;

    // 释放所有 Tracked 内存块
    RTBlock* currentTracked = HeadSmallTarget;
    while (currentTracked)
    {
        RTBlock* next = currentTracked->pNext;
        RTSysFree(currentTracked, currentTracked->Size + sizeof(RTBlock)); // 释放 RTBlock 结构和用户内存
        currentTracked = next;
    }
}

void RTDebugHeapMemoryManager::TrackAllocation(void* Ptr, size_t Size, const char* File, uint32_t Line)
{
    if (!Ptr) return; // 空指针直接返回

    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全

    RTBlock* block = reinterpret_cast<RTBlock*>(Ptr) - 1; // 指针回退到 RTBlock 结构起始位置
    block->File = File;
    block->Line = Line;
}

void RTDebugHeapMemoryManager::UntrackAllocation(void* Ptr)
{
    //  在 deallocateImpl 中处理，这里无需额外操作
}

void RTDebugHeapMemoryManager::DumpMemoryLeaks()
{
    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全

    std::cout << "---------- 内存泄漏检测报告 ----------" << std::endl;
    RTBlock* current = HeadSmallTarget;
    size_t leakCount = 0;
    while (current)
    {
        std::cout << "检查内存地址: =" << current << std::endl;
        std::cout << "检查内存状态: =" << current->Size << std::endl;
        size_t metadataSize = current->IsArray ? sizeof(size_t) : 0;
        EBlock* beginMaskPtr = reinterpret_cast<EBlock*>(reinterpret_cast<char*>(current) + sizeof(RTBlock));
        EBlock* endMaskPtr = reinterpret_cast<EBlock*>(reinterpret_cast<char*>(current) + sizeof(RTBlock) + sizeof(EBlock) + current->Size + metadataSize);
        std::cout << "检查内存状态: =" << beginMaskPtr << std::endl;
        std::cout << "检查内存状态: =" << endMaskPtr << std::endl;
        if( *beginMaskPtr != EBlock::BEGIN_MASK ||
            *endMaskPtr != EBlock::END_MASK)
        {
            std::cout << "内存泄漏: 大小=" << current->Size<< std::endl;
            leakCount++;
        }
        current = current->pNext;
    }

    if (leakCount == 0)
    {
        std::cout << "恭喜，没有检测到内存泄漏！" << std::endl;
    }
    else
    {
        std::cout << "总共检测到 " << leakCount << " 处内存泄漏。" << std::endl;
    }
    std::cout << "---------- 内存泄漏检测报告结束 ----------" << std::endl;
}

void* RTDebugHeapMemoryManager::allocatePoolCall(uint32_t size, uint32_t alignment, bool isArray)
{
    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全

    if (!HeadSmallTarget)
    {
        std::cerr << "RTDebugHeapMemoryManager::allocateImpl 内存池已耗尽!" << std::endl;
        return nullptr; // 内存池耗尽
    }

    // 1. 从 Tracked 链表头部取出一个空闲 RTBlock
    RTBlock* block = HeadSmallTarget;
    HeadSmallTarget = HeadSmallTarget->pNext;
    if (HeadSmallTarget)
    {
        HeadSmallTarget->pPrev = nullptr;
    }
    else
    {
        TailSmallTarget = nullptr; // 链表为空
    }

    // 2. 更新内存统计信息 (分配时不再增加 TotalAllocatedMemory，因为内存已预先分配)
    // TotalAllocatedMemory += (block->Size + sizeof(RTBlock)); //  预分配模式下，TotalAllocatedMemory 在构造函数中已计算
    // if (TotalAllocatedMemory > PeakAllocatedMemory)
    // {
    //     PeakAllocatedMemory = TotalAllocatedMemory;
    // }

    // 3. 返回用户可用的内存地址 (跳过 RTBlock 结构)
    block->Size = size; // 记录实际分配的大小 (虽然内存池块大小固定，但用户请求的大小可能略小)
    return reinterpret_cast<void*>(block + 1); // 返回 RTBlock 结构之后的位置，即用户可用的内存起始地址
}

void RTDebugHeapMemoryManager::deAllocatePoolCall(void* addr, uint32_t alignment, bool isArray)
{
        if (!addr) return; // 空指针直接返回

    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全

    RTBlock* block = reinterpret_cast<RTBlock*>(addr) - 1; // 指针回退到 RTBlock 结构起始位置
    if (block->Size == 0) return; // 防止重复释放或释放非法内存

    // 1. 将 RTBlock 放回 Tracked 链表头部
    block->pNext = HeadSmallTarget;
    block->pPrev = nullptr;
    if (HeadSmallTarget)
    {
        HeadSmallTarget->pPrev = block;
    }
    HeadSmallTarget = block;
    if (!TailSmallTarget)
    {
        TailSmallTarget = block; // 如果之前链表为空，则更新 TailSmallTarget
    }

    // 2. 更新内存统计信息 (释放时不再减少 TotalAllocatedMemory，因为内存是预先分配的)
    block->Size = 0; // 标记为已释放，Size 清零，用于内存泄漏检测判断

    // 3.  不再调用 RTSysFree，内存块放回内存池，下次可重用
    // RTSysFree(block, block->Size + sizeof(RTBlock)); //  内存池模式下，不再释放回系统
}

void* RTDebugHeapMemoryManager::allocateSystemCall(uint32_t size, uint32_t alignment, bool isArray)
{
    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全
    printf("Allocate, size:%d, alignment:%d\n", size, alignment);

    // 1. 先分配 RTBlock 结构 + 用户请求的内存
    size_t metadataSize = isArray ? sizeof(size_t) : 0;
    size_t totalSize = size + sizeof(RTBlock) + sizeof(EBlock) + sizeof(EBlock) + metadataSize;
    printf("RTSysAlloc 申请值: %zu\n", totalSize); // 立即打印 RTSysAlloc 的返回值
    printf("RTSysAlloc metadataSize: %zu\n", metadataSize); // 立即打印 RTSysAlloc 的返回值
    char* ptr = static_cast<char*>(RTSysAlloc(totalSize));
    printf("RTSysAlloc 返回值: %p\n", ptr); // 立即打印 RTSysAlloc 的返回值
    if (!ptr)
    {
        std::cerr << "RTDebugHeapMemoryManager::allocateImpl 内存分配失败!" << std::endl;
        return nullptr; // 分配失败
    }
    printf("内存起始地址: %p\n", ptr); // 立即打印 RTSysAlloc 的返回值
    RTBlock* block = reinterpret_cast<RTBlock*>(ptr);

    // 2. 初始化 RTBlock 信息
    new (block) RTBlock(); // 使用 placement new 初始化 RTBlock 结构体，避免覆盖分配的内存
    block->Size = size;
    block->Alignment = alignment;
    block->IsArray = isArray;
    // 3. 将 RTBlock 加入 Tracked 链表头部 (作为 Tracked 内存管理)
    block->pNext = HeadSmallTarget;
    block->pPrev = nullptr;
    if (HeadSmallTarget)
    {
        HeadSmallTarget->pPrev = block;
    }
    HeadSmallTarget = block;
    if (!TailSmallTarget)
    {
        TailSmallTarget = block;
    }

    ptr += sizeof(RTBlock);
    EBlock* startMaskPtr = reinterpret_cast<EBlock*>(ptr);
    *startMaskPtr = EBlock::BEGIN_MASK;

    ptr += sizeof(EBlock) + metadataSize;

    EBlock* endMaskPtr = reinterpret_cast<EBlock*>(ptr + size);
    *endMaskPtr = EBlock::END_MASK;

        std::cout << "检查内存状态: =" << startMaskPtr << std::endl;
        std::cout << "检查内存状态: =" << endMaskPtr << std::endl;

    // 4. 更新内存统计信息
    TotalAllocatedMemory += totalSize;
    if (TotalAllocatedMemory > PeakAllocatedMemory)
    {
        PeakAllocatedMemory = TotalAllocatedMemory;
    }

    // 5. 返回用户可用的内存地址 (跳过 RTBlock 结构)
    printf("可用内存起始地址: %p\n", ptr); // 立即打印 RTSysAlloc 的返回值
    // DumpMemoryLeaks();
    return reinterpret_cast<void*>(ptr); // 返回 RTBlock 结构之后的位置，即用户可用的内存起始地址
}

void RTDebugHeapMemoryManager::deAllocateSystemCall(void* addr, uint32_t alignment, bool isArray)
{
    if (!addr) return; // 空指针直接返回

    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全
    printf("DeAllocate, addr:%d, alignment:%d\n", addr, alignment);
    size_t metadataSize = isArray ? sizeof(size_t) : 0;
    RTBlock* block = reinterpret_cast<RTBlock*>((char*)addr - (sizeof(EBlock) + sizeof(RTBlock) + metadataSize)); // 指针回退到 RTBlock 结构起始位置
    if (block->Size == 0) return; // 防止重复释放或释放非法内存

    // 1. 从 Tracked 链表中移除 RTBlock
    if (block->pPrev)
    {
        block->pPrev->pNext = block->pNext;
    }
    else
    {
        HeadSmallTarget = block->pNext; // 更新 HeadSmallTarget
    }

    if (block->pNext)
    {
        block->pNext->pPrev = block->pPrev;
    }
    else
    {
        TailSmallTarget = block->pPrev; // 更新 TailSmallTarget
    }

    // 2. 更新内存统计信息
    TotalAllocatedMemory -= (block->Size + sizeof(RTBlock) + sizeof(EBlock) + sizeof(EBlock) + metadataSize);
    size_t size = block->Size + sizeof(RTBlock) + sizeof(EBlock) + sizeof(EBlock) + metadataSize;
    // 3. 清理 RTBlock 标记 (可选，如果需要更严格的调试)
    block->Size = 0; // 标记为已释放，防止 double free 等错误

    // 4. 释放内存 (RTBlock 结构 + 用户内存)
    RTSysFree(block, size);
}

void* RTDebugHeapMemoryManager::allocateImpl(size_t size, uint8_t alignment, bool isArray)
{
    return allocateSystemCall(size, alignment, isArray);
}
void RTDebugHeapMemoryManager::deallocateImpl(void* ptr, uint8_t alignment, bool isArray)
{
    return deAllocateSystemCall(ptr, alignment, isArray);
}
// template class RTSingleton<RTMemoryManager<RTDebugHeapMemoryManager>>;
}
