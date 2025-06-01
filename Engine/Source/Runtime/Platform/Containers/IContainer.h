#ifndef PLATFORM_CONTAINERS_ICONTAINER_H
#define PLATFORM_CONTAINERS_ICONTAINER_H
#include "Core/MinimalCore.h"
#include "../Memory/Memory.h"
namespace ReiToEngine
{
    template <typename T>
    class IContainer
    {
    protected:
        T* New(u64 size)
        {
            if (!size)
			{
				return nullptr;
			}
			T* pPtr = (T*)GetMemoryManager().Allocate(size * sizeof(T), 8, RT_MEMORY_TAG::CONTAINER);
			RT_ASSERT(pPtr);

			return pPtr;
        }

        void Delete(T* ptr, u64 size)
        {
			if (!ptr)
			{
				return ;
			}
			if (size > 0)
			{
				// if (ValueBase<T>::NeedsDestructor)
				if (true)
				{
					for (u64 i = 0 ; i < size; ++i)
					{
						(ptr + i)->~T();
					}
				}
			}

            GetMemoryManager().Free(ptr, size, RT_MEMORY_TAG::CONTAINER);
			pPtr = nullptr;
        }
    };
}

#endif
