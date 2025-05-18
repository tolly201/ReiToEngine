#ifndef CORE_PLATFORM_MEMORY_IMEMORYALLOCATOR_H
#define CORE_PLATFORM_MEMORY_IMEMORYALLOCATOR_H
#include <cstdint>
#include <cstddef>
#include "Core/MinimalCore.h"
namespace ReiToEngine
{
template <typename T>
class RTENGINE_API RTMemoryManager
{
public:
    RTMemoryManager() = default;
    virtual ~RTMemoryManager() = 0;
    void* Allocate(size_t size, uint8_t alignment, bool isArray)
    {
        return static_cast<T*>(this)->allocateImpl(size, alignment, isArray);
    }
    void Deallocate(void* addr, uint8_t alignment, bool isArray)
    {
        static_cast<T*>(this)->deallocateImpl(addr, alignment, isArray);
    }
protected:
    virtual void* allocateImpl(size_t, uint8_t, bool) = 0;
    virtual void deallocateImpl(void*, uint8_t, bool) = 0;
};

template <typename T>
RTMemoryManager<T>::~RTMemoryManager() = default;
}



#endif
