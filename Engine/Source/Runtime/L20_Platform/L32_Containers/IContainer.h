#ifndef PLATFORM_CONTAINERS_ICONTAINER_H
#define PLATFORM_CONTAINERS_ICONTAINER_H
#include "L0_Macro/Include.h"
#include "L20_Platform/L23_Logger/Include.h"
#include "L20_Platform/L31_SingletonFactory/SingletonFactory.h"
namespace ReiToEngine
{
    template <typename T>
    class IContainer
    {
    protected:
        T* New(u64 size, RT_MEMORY_TAG tag = RT_MEMORY_TAG::CONTAINER)
        {
            if (!size)
			{
				return nullptr;
			}
			// Allocate expects bytes; use element count * sizeof(T), with proper alignment.
			T* pPtr = (T*)GetMemoryManager().Allocate(size * sizeof(T), alignof(T), tag);
			RT_ASSERT(pPtr);

			return pPtr;
        }

		// Delete with element count = used_count, and free bytes = used_count * sizeof(T).
		void Delete(T* ptr, u64 used_count,  RT_MEMORY_TAG tag = RT_MEMORY_TAG::CONTAINER)
        {
			if (!ptr)
			{
				return ;
			}
			if (used_count > 0)
			{
				// if (ValueBase<T>::NeedsDestructor)
				if (true)
				{
					for (u64 i = 0 ; i < used_count; ++i)
					{
						(ptr + i)->~T();
					}
				}
			}

			// Free expects bytes; here we free exactly used_count elements worth of bytes.
			GetMemoryManager().Free(ptr, used_count * sizeof(T), tag);
			ptr = nullptr;
        }

		// Overload allowing separate used_count (destruct N elements) and allocated_count (free bytes).
		void Delete(T* ptr, u64 used_count, u64 allocated_count, RT_MEMORY_TAG tag)
		{
			if (!ptr)
			{
				return;
			}
			if (used_count > 0)
			{
				if (true)
				{
					for (u64 i = 0 ; i < used_count; ++i)
					{
						(ptr + i)->~T();
					}
				}
			}
			GetMemoryManager().Free(ptr, allocated_count * sizeof(T), tag);
		}
    };
}

#endif
