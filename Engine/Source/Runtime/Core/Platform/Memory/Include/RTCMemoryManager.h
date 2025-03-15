#ifndef CORE_HAL_MEMORY_RTCMEMORYALLOCATOR_H
#define CORE_HAL_MEMORY_RTCMEMORYALLOCATOR_H
#include "IMemoryManager.h"
namespace ReiToEngine
{
class RTENGINE_API RTCMemoryManager : public RTMemoryManager<RTCMemoryManager>
{
friend class RTMemoryManager<RTCMemoryManager>;
public:
    ~RTCMemoryManager();
protected:
    void* allocateImpl(size_t, uint8_t, bool) override;
    void deallocateImpl(void*, uint8_t, bool) override;
private:
    RTCMemoryManager() = default;
};
}

#endif
