#ifndef CORE_PLATFORM_MEMORY_IBLOCK_H
#define CORE_PLATFORM_MEMORY_IBLOCK_H
#include "Core/MinimalCore.h"
#include <cstdint>
namespace ReiToEngine
{

enum EBlock : u64
{
	BEGIN_MASK = 0xDEADC0DE,
	END_MASK = 0xC0DEDEAD,
	RECORD_NUM = 32,
	CALLSTACK_NUM = 32
};

enum RT_MEMORY_TAG : u16
{
    UNKNOWN = 0,
    ARRAY = 1,
    DARRAY = 2,
    DICT = 3,
    QUEUE = 4,
    BST = 5,
    STRING = 6,
    APPLICATION = 7,
    JOB = 8,
    TEXTURE = 9,
    MATERIAL_INSTANCE = 10,
    RENDERER = 11,
    GAME = 12,
    TRANSFORM = 13,
    ENTITY = 14,
    ENTITT_NODE = 15,
    SCENE = 16,
    AUDIO = 17,
    MAX_TAG = 18
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
