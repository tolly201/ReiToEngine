#ifndef REITOENGINE_PLATFORM_MEMORY_ICUSTOMMEMORYMANAGER_H
#define REITOENGINE_PLATFORM_MEMORY_ICUSTOMMEMORYMANAGER_H

#include "L0_Macro/Defines.h"
#include "L0_Macro/Macro.h"
#include "IBlock.h" // for RT_MEMORY_TAG if defined there
#include "IMemoryManager.h"

namespace ReiToEngine {

class ICustomMemoryManager : public RTMemoryManager<ICustomMemoryManager>
{
friend class RTMemoryManager<ICustomMemoryManager>;
public:
    virtual ~ICustomMemoryManager() = default;
    ICustomMemoryManager() = default;
protected:
    virtual void* AllocateImpl(u64, u8, RT_MEMORY_TAG) override{return nullptr;};
    virtual void FreeImpl(void*, u64, RT_MEMORY_TAG) override{
        return;
    };
    virtual void* ZeroMemoryImpl(void*, u64) override{return nullptr;};
    virtual void* CopyMemoryImpl(void*, const void*, u64) override{return nullptr;};
    virtual void* SetMemoryImpl(void*, u8, u64) override{return nullptr;};
};
}

#endif // REITOENGINE_PLATFORM_MEMORY_ICUSTOMMEMORYMANAGER_H
