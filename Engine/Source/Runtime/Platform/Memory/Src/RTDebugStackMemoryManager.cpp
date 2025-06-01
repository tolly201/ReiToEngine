#include "../Include/RTDebugStackMemoryManager.h"
#include "../Include/MemoryAllocator_cpp.h" // 引入 RTSysAlloc 和 RTSysFree
#include <iostream>
namespace ReiToEngine
{
RTDebugStackMemoryManager::RTDebugStackMemoryManager() :
    HeadSmallTarget(nullptr),
    TailSmallTarget(nullptr),
    TotalAllocatedMemory(0),
    PeakAllocatedMemory(0)
{

}

RTDebugStackMemoryManager::~RTDebugStackMemoryManager()
{
    DumpMemoryLeaks(); // 析构时自动检测内存泄漏
    std::cout << "RTDebugStackMemoryManager 析构函数" << std::endl;

    // 释放所有 Tracked 内存块
    RTBlock* currentTracked = HeadSmallTarget;
    while (currentTracked)
    {
        RTBlock* next = currentTracked->pNext;
        RTSysFree(currentTracked, currentTracked->Size + sizeof(RTBlock)); // 释放 RTBlock 结构和用户内存
        currentTracked = next;
    }
}

void RTDebugStackMemoryManager::TrackAllocation(void* Ptr, size_t Size, const char* File, uint32_t Line)
{
    if (!Ptr) return; // 空指针直接返回

    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全

    RTBlock* block = reinterpret_cast<RTBlock*>(Ptr) - 1; // 指针回退到 RTBlock 结构起始位置
    block->File = File;
    block->Line = Line;
}

void RTDebugStackMemoryManager::UntrackAllocation(void* Ptr)
{
    //  在 deallocateImpl 中处理，这里无需额外操作
}

void RTDebugStackMemoryManager::DumpMemoryLeaks()
{
    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全

    std::cout << "---------- 内存泄漏检测报告 ----------" << std::endl;
    RTBlock* current = HeadSmallTarget;
    size_t leakCount = 0;
    while (current)
    {
        std::cout << "内存泄漏: 大小=" << current->Size << " 字节, 文件=" << current->File << ", 行号=" << current->Line << std::endl;
        leakCount++;
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

void* RTDebugStackMemoryManager::allocatePoolCall(uint32_t size, uint32_t alignment)
{
     if (size > defaultSmallTargetMemoryAlignment) // 内存池只处理固定大小的内存块
    {
        std::cerr << "RTDebugStackMemoryManager::allocateImpl 请求分配大小超过内存池块大小，不支持!" << std::endl;
        return nullptr; //  或者可以回退到 RTSysAlloc 分配
    }

    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全

    if (!HeadSmallTarget)
    {
        std::cerr << "RTDebugStackMemoryManager::allocateImpl 内存池已耗尽!" << std::endl;
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

void RTDebugStackMemoryManager::deAllocatePoolCall(void* addr, uint32_t alignment)
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

void* RTDebugStackMemoryManager::allocateSystemCall(uint32_t size, uint32_t alignment)
{
    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全

    // 1. 先分配 RTBlock 结构 + 用户请求的内存
    size_t totalSize = size + sizeof(RTBlock);
    RTBlock* block = reinterpret_cast<RTBlock*>(RTSysAlloc(totalSize));
    if (!block)
    {
        std::cerr << "RTDebugStackMemoryManager::allocateImpl 内存分配失败!" << std::endl;
        return nullptr; // 分配失败
    }

    // 2. 初始化 RTBlock 信息
    new (block) RTBlock(); // 使用 placement new 初始化 RTBlock 结构体，避免覆盖分配的内存
    block->Size = size;
    block->Alignment = alignment;

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

    // 4. 更新内存统计信息
    TotalAllocatedMemory += totalSize;
    if (TotalAllocatedMemory > PeakAllocatedMemory)
    {
        PeakAllocatedMemory = TotalAllocatedMemory;
    }

    // 5. 返回用户可用的内存地址 (跳过 RTBlock 结构)
    return reinterpret_cast<void*>(block + sizeof(EBlock) + sizeof(block)); // 返回 RTBlock 结构之后的位置，即用户可用的内存起始地址
}

void RTDebugStackMemoryManager::deAllocateSystemCall(void* addr, uint32_t alignment)
{
    if (!addr) return; // 空指针直接返回

    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全

    RTBlock* block = reinterpret_cast<RTBlock*>(addr) - 1; // 指针回退到 RTBlock 结构起始位置
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
    TotalAllocatedMemory -= (block->Size + sizeof(RTBlock));

    // 3. 清理 RTBlock 标记 (可选，如果需要更严格的调试)
    block->Size = 0; // 标记为已释放，防止 double free 等错误

    // 4. 释放内存 (RTBlock 结构 + 用户内存)
    RTSysFree(block, block->Size + sizeof(RTBlock));
}

void* RTDebugStackMemoryManager::AllocateImpl(u64, u8, RT_MEMORY_TAG){
    return nullptr;
};
void RTDebugStackMemoryManager::FreeImpl(void*, u64, RT_MEMORY_TAG){
};
void* RTDebugStackMemoryManager::ZeroMemoryImpl(void*, u64){
    return nullptr;
};
void* RTDebugStackMemoryManager::CopyMemoryImpl(void*, const void*, u64){
    return nullptr;
};
void* RTDebugStackMemoryManager::SetMemoryImpl(void*, u8, u64){
    return nullptr;
};
}
