#ifndef PLATFORM_MEMORY_MANAGER_IBLOCK_H
#define PLATFORM_MEMORY_MANAGER_IBLOCK_H
#include "L0_Macro/Include.h"
namespace ReiToEngine
{

enum class EBLOCK : u64
{
	BEGIN_MASK = 0xDEADC0DE,
	END_MASK = 0xC0DEDEAD,
	RECORD_NUM = 32,
	CALLSTACK_NUM = 32
};

enum class RT_MEMORY_TAG : u16
{
    UNKNOWN = 0,
    CONTAINER = 1,
    ARRAY = 2,
    DARRAY = 3,
    DICT = 4,
    QUEUE = 5,
    BST = 6,
    STRING = 7,
    APPLICATION = 8,
    JOB = 9,
    TEXTURE = 10,
    MATERIAL_INSTANCE = 11,
    RENDERER = 12,
    GAME = 13,
    TRANSFORM = 14,
    ENTITY = 15,
    ENTITT_NODE = 16,
    SCENE = 17,
    AUDIO = 18,

    SINGLETON = 19,
    MATH = 20,
    MAX_TAG = 21
};

class RTBlock
{
public:
    RTBlock()
    {
        for (u32 i = 0; i < static_cast<u64>(EBLOCK::CALLSTACK_NUM); ++i)
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
    void* pAddr[static_cast<u64>(EBLOCK::CALLSTACK_NUM)];
    u32 StackInfoNum;
    u32 Size;
    bool IsArray;
    u32 Alignment;
    RTBlock* pPrev;
    RTBlock* pNext;

    // 调试信息
    const char* File;
    u32 Line;
};
}
#endif
