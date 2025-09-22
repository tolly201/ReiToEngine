#ifndef PLATFORM_MIMALLOC_MANAGER_H
#define PLATFORM_MIMALLOC_MANAGER_H
#include "IMemoryManager.h"
namespace ReiToEngine
{
class RTMimallocManager : public RTMemoryManager<RTMimallocManager>
{
    friend class RTMemoryManager<RTMimallocManager>;
public:
    RTMimallocManager();
    ~RTMimallocManager();
protected:
    void* AllocateImpl(u64, u8, RT_MEMORY_TAG) override;
    void FreeImpl(void*, u64, RT_MEMORY_TAG) override;
    void* ZeroMemoryImpl(void*, u64) override;
    void* CopyMemoryImpl(void*, const void*, u64) override;
    void* SetMemoryImpl(void*, u8, u64) override;
public:
};
}

#endif
