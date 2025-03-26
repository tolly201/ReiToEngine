#ifndef CORE_PLATFORM_MEMORY_IBLOCK_H
#define CORE_PLATFORM_MEMORY_IBLOCK_H
#include "Core/Macro/Macro.h"
#include <cstdint>
namespace ReiToEngine
{
enum EBlock : uint64_t
{
	BEGIN_MASK = 0xDEADC0DE,
	END_MASK = 0xC0DEDEAD,
	RECORD_NUM = 32,
	CALLSTACK_NUM = 32
};
class RTBlock
{
public:
    RTBlock()
    {
        for (uint32_t i = 0; i < CALLSTACK_NUM; ++i)
        {
            pAddr[i] = nullptr;
        }
        pPrev = nullptr;
        pNext = nullptr;
        Size = 0;
        IsArray = false;
        Alignment = 0;
        StackInfoNum = 0;
    }

    // 内存块信息
    void* pAddr[CALLSTACK_NUM];
    uint32_t StackInfoNum;
    uint32_t Size;
    bool IsArray;
    uint32_t Alignment;
    RTBlock* pPrev;
    RTBlock* pNext;

    // 调试信息
    const char* File;
    uint32_t Line;
};
}
#endif
