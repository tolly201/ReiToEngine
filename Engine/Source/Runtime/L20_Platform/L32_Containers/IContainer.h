#ifndef PLATFORM_CONTAINERS_ICONTAINER_H
#define PLATFORM_CONTAINERS_ICONTAINER_H
#include "L0_Macro/Include.h"
#include "../L31_SingletonFactory/SingletonFactory.h"
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
			T* pPtr = (T*)GetMemoryManager().Allocate(size * sizeof(T), 8, tag);
			RT_ASSERT(pPtr);

			return pPtr;
        }

        void Delete(T* ptr, u64 size,  RT_MEMORY_TAG tag = RT_MEMORY_TAG::CONTAINER)
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

            GetMemoryManager().Free(ptr, size, tag);
			ptr = nullptr;
        }
    };
}

#endif
