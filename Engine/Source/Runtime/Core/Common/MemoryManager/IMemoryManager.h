#ifndef CORE_COMMON_MEMORYMANAGER_MEMORYMANAGER_H
#define CORE_COMMON_MEMORYMANAGER_MEMORYMANAGER_H

#include <iostream>
#include "Core/HAL/Platform.h"
namespace ReiToEngine
{
template <typename T>
class RTENGINE_API RTMemoryManager
{
public:
    RTMemoryManager() = default;
    virtual ~RTMemoryManager() = 0; // 声明
    void* Allocate(uint32_t size, uint32_t alignment)
    {
        return static_cast<T*>(this)->allocateImpl(size, alignment);
    }
    void Deallocate(void* addr, uint32_t alignment)
    {
        static_cast<T*>(this)->deallocateImpl(addr, alignment);   
    }
protected:
    virtual void* allocateImpl(uint32_t, uint32_t) = 0;
    virtual void deallocateImpl(void*, uint32_t) = 0; 
};

class RTENGINE_API RTCMemoryManager : public RTMemoryManager<RTCMemoryManager>
{
friend class RTMemoryManager<RTCMemoryManager>;
public:    
    static RTCMemoryManager& GetInstance() {
        static RTCMemoryManager instance;
        return instance;
    }
    ~RTCMemoryManager();
protected:
    void* allocateImpl(uint32_t size, uint32_t alignment) override; // Ensure this is correctly declared
    void deallocateImpl(void* addr, uint32_t alignment) override; 
private:
    RTCMemoryManager() = default;
};

class RTENGINE_API RTDebugStackMemoryManager : public RTMemoryManager<RTDebugStackMemoryManager>
{
    
};
}

#endif
