#include "../Include/RTReleaseHeapMemoryManager.h"
#include <cassert>

namespace ReiToEngine
{
template< class T > inline T Align(const T Ptr, uint32_t Alignment )
{
	return (T)(((uint32_t)Ptr + Alignment - 1) & ~(Alignment-1));
}

RTReleaseHeapMemoryManager::RTReleaseHeapMemoryManager()
{
	SystemPageSize = 0 ;
	SystemPageSize = 64;
	assert((SystemPageSize&(SystemPageSize-1)) == 0 && "SystemPageSize is not 2^n");

	// Init tables.
	OsTable.FirstPool    = nullptr;
	OsTable.ExhaustedPool = nullptr;
	OsTable.BlockSize    = 0;

	PoolTable[0].FirstPool    = nullptr;
	PoolTable[0].ExhaustedPool = nullptr;
	PoolTable[0].BlockSize    = 8;
	for( size_t i=1; i<5; i++ )
	{
		PoolTable[i].FirstPool    = nullptr;
		PoolTable[i].ExhaustedPool = nullptr;
		PoolTable[i].BlockSize    = (8<<((i+1)>>2)) + (2<<i);
	}
	for( size_t i=5; i<POOL_COUNT; i++ )
	{
		PoolTable[i].FirstPool    = nullptr;
		PoolTable[i].ExhaustedPool = nullptr;
		PoolTable[i].BlockSize    = (4+((i+7)&3)) << (1+((i+7)>>2));
	}
	for( size_t i=0; i < POOL_MAX; i++ )
	{
		size_t Index;
		for( Index=0; PoolTable[Index].BlockSize<i; Index++ );
		assert(Index < POOL_COUNT && "POOL MAX OVERSIZE.");
		MemSizeToPoolTable[i] = &PoolTable[Index];
	}
	for(size_t i=0; i < 32 ; i++)
	{
		PoolIndirect[i] = nullptr;
	}
	assert(POOL_MAX - 1 == PoolTable[POOL_COUNT - 1].BlockSize && "Pool Max Size Error. Initialize Memory Pool Failed.");
}

RTReleaseHeapMemoryManager::~RTReleaseHeapMemoryManager()
{
	for (unsigned int i = 0 ; i < 32 ; i++)
	{
		for (unsigned int j = 0 ; j < 2048 ; j++)
		{
			if (PoolIndirect[i])
			{
				if (PoolIndirect[i][j].Mem)
				{
					RT_HAL_SYSFree( PoolIndirect[i][j].Mem, PoolIndirect[i][j].Bytes);
					PoolIndirect[i][j].Mem = nullptr;
				}

				RT_HAL_SYSFree( PoolIndirect[i][j].Mem, PoolIndirect[i][j].Bytes);
				PoolIndirect[i] = nullptr;
			}
		}
	}
}

FPoolInfo* FFreeMem::GetPool()
{
    // return nullptr;
	return (FPoolInfo*)((uintptr_t)this & 0xffff0000);
}
void* RTReleaseHeapMemoryManager::ZeroMemoryImpl(void*, u64) {};
void* RTReleaseHeapMemoryManager::CopyMemoryImpl(void*, const void*, u64) {};
void* RTReleaseHeapMemoryManager::SetMemoryImpl(void*, u8, u64) {};

void* RTReleaseHeapMemoryManager::AllocateImpl(u64 uiSize, uint8_t u8, RT_MEMORY_TAG tag)
{
	std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全
	FFreeMem* Free;
	if( uiSize<POOL_MAX )
	{
		// Allocate from pool.
		FPoolTable* Table = MemSizeToPoolTable[uiSize];
		assert(uiSize<=Table->BlockSize && "Memory Size To Pool Check Error.");
		FPoolInfo* Pool = Table->FirstPool;
		if( !Pool )
		{
			// Must create a new pool.
			uint32_t Blocks  = 65536 / Table->BlockSize;
			uint32_t Bytes   = Blocks * Table->BlockSize;
			assert(Blocks>=1 && "Blocks Counting Error.");
			assert(Blocks*Table->BlockSize<=Bytes && "Bytes Counting Error.");

			// Allocate memory.
			Free = (FFreeMem*)RT_HAL_SYSAlloc(Bytes);
			if( !Free )
			{
				return nullptr;
			}

			// Create pool in the indirect table.
			FPoolInfo*& Indirect = PoolIndirect[((uintptr_t)Free>>27)];
			if( !Indirect )
			{
				Indirect = CreateIndirect();
			}
			Pool = &Indirect[((uintptr_t)Free>>16)&2047];

			// Init pool.
			Pool->Link( Table->FirstPool );
			Pool->Mem            = (void*)Free;
			Pool->Bytes		     = Bytes;
			Pool->OsBytes		 = Align(Bytes, SystemPageSize);
			Pool->Table		     = Table;
			Pool->RefCount			 = 0;
			Pool->FirstMem       = Free;

			// Create first free item.
			Free->Blocks         = Blocks;
			Free->Next           = NULL;
		}

		// Pick first available block and unlink it.
		Pool->RefCount++;
		assert(Pool->FirstMem && "allocation failed.");
		assert(Pool->FirstMem->Blocks > 0 && "allocation failed.");
		Free = (FFreeMem*)((char*)Pool->FirstMem + --Pool->FirstMem->Blocks * Table->BlockSize);
		if( Pool->FirstMem->Blocks==0 )
		{
			Pool->FirstMem = Pool->FirstMem->Next;
			if( !Pool->FirstMem )
			{
				// Move to exhausted list.
				Pool->Unlink();
				Pool->Link( Table->ExhaustedPool );
			}
		}
	}
	else
	{
		// Use OS for large allocations.
		size_t AlignedSize = Align(uiSize,SystemPageSize);
		Free = (FFreeMem*)RT_HAL_SYSAlloc(AlignedSize);
		if( !Free )
		{
			return nullptr;
		}
		assert(!((size_t)Free&65535));

		// Create indirect.
		FPoolInfo*& Indirect = PoolIndirect[((uintptr_t)Free>>27)];
		if( !Indirect )
		{
			Indirect = CreateIndirect();
		}

		// Init pool.
		FPoolInfo* Pool = &Indirect[((uintptr_t)Free>>16)&2047];
		Pool->Mem		= (void*)Free;
		Pool->Bytes		= uiSize;
		Pool->OsBytes	= AlignedSize;
		Pool->Table		= &OsTable;

	}
	return Free;
}
void RTReleaseHeapMemoryManager::FreeImpl(void* pcAddr, u64 size,RT_MEMORY_TAG tag)
{
    std::lock_guard<std::mutex> lock(MemoryMutex); // 加锁保证线程安全
	assert(pcAddr);
	// Windows version.
	FPoolInfo* Pool = &PoolIndirect[(uintptr_t)pcAddr>>27][((uintptr_t)pcAddr>>16)&2047];
	assert(Pool->Bytes!=0);
	if( Pool->Table!=&OsTable )
	{
		// If this pool was exhausted, move to available list.
		if( !Pool->FirstMem )
		{
			Pool->Unlink();
			Pool->Link( Pool->Table->FirstPool );
		}

		// Free a pooled allocation.
		FFreeMem* Free		= (FFreeMem *)pcAddr;
		Free->Blocks		= 1;
		Free->Next			= Pool->FirstMem;
		Pool->FirstMem		= Free;


		// Free this pool.
		assert(Pool->RefCount>=1);
		if( --Pool->RefCount == 0 )
		{
			// Free the OS memory.
			Pool->Unlink();
			RT_HAL_SYSFree( Pool->Mem, 0);
			Pool->Mem = NULL;
		}
	}
	else
	{
		// Free an OS allocation.

		RT_HAL_SYSFree( pcAddr, 0);
		Pool->Mem = NULL;
	}

}
}
