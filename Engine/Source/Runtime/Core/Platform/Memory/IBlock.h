#ifndef CORE_COMMON_MEMORYMANAGER_MEMORYMANAGER_H
#define CORE_COMMON_MEMORYMANAGER_MEMORYMANAGER_H
#include "Core/HAL/Platform.h"
namespace ReiToEngine
{
enum EBlock
{
	BEGIN_MASK = 0xDEADC0DE,
	END_MASK = 0xDEADC0DE,
	RECORD_NUM = 32,
	CALLSTACK_NUM = 32
};
class RTENGINE_API RTBlock
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
    }
	void * pAddr[CALLSTACK_NUM];
	uint32_t m_uiStackInfoNum;
	uint32_t m_uiSize;
	bool IsArray;
	bool Alignment;
	RTBlock* pPrev;
	RTBlock* pNext;
};
}
#endif