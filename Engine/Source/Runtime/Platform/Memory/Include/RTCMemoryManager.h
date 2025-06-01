#ifndef CORE_HAL_MEMORY_RTCMEMORYALLOCATOR_H
#define CORE_HAL_MEMORY_RTCMEMORYALLOCATOR_H
#include "IMemoryManager.h"
namespace ReiToEngine
{
class RTENGINE_API RTCMemoryManager : public RTMemoryManager<RTCMemoryManager>
{
friend class RTMemoryManager<RTCMemoryManager>;
public:
    ~RTCMemoryManager() = default;
    RTCMemoryManager() = default;
protected:
    void* AllocateImpl(u64, u8, RT_MEMORY_TAG) override;
    void FreeImpl(void*, u64, RT_MEMORY_TAG) override;
    void* ZeroMemoryImpl(void*, u64) override;
    void* CopyMemoryImpl(void*, const void*, u64) override;
    void* SetMemoryImpl(void*, u8, u64) override;
private:
};
}

#endif
