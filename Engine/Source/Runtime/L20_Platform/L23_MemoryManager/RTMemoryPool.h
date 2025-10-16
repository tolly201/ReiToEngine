#ifndef CORE_HAL_MEMORY_RTMEMORYPOOL_H
#define CORE_HAL_MEMORY_RTMEMORYPOOL_H
#include "L0_Macro/Include.h"
#include <cstddef>
#include <cstdint>
namespace ReiToEngine
{
    struct FFreeMem;
    struct FPoolTable;

    struct FPoolInfo
    {
        uint64_t Bytes;
        uint8_t OsBytes;
        size_t RefCount;
        void *Mem;
        FPoolTable *Table;
        FFreeMem *FirstMem;
        FPoolInfo *Next;
        FPoolInfo **PrevLink;

        void Link(FPoolInfo *&Before)
        {
            if (Before)
            {
                Before->PrevLink = &Next;
            }
            Next = Before;
            PrevLink = &Before;
            Before = this;
        }
        void Unlink()
        {
            if (Next)
            {
                Next->PrevLink = PrevLink;
            }
            *PrevLink = Next;
        }
    };

    struct FFreeMem
    {
        FFreeMem *Next; // Next or MemLastPool[], always in order by pool.
        size_t Blocks;  // Number of consecutive free blocks here, at least 1.
        FPoolInfo *GetPool();
    };

    struct FPoolTable
    {
        FPoolInfo *FirstPool;
        FPoolInfo *ExhaustedPool;
        uint32_t  BlockSize;
    };
}

#endif
