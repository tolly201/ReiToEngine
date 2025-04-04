#include "../Include/RTApplication.h"
#include "Platform/HAL/System/Include/SystemInfo.h"
#include "Platform/WindowsManager/Include/WindowsManager.h"

namespace ReiToEngine{
    RTApplication::RTApplication() = default;
    RTApplication::~RTApplication() = default;
    RTDebugHeapMemoryManager& RTApplication::GetMemoryManager()
    {
        return RTApplication::memoryManager;
    }

    RTApplication& RTApplication::Instance()
    {
        return *instance_ptr;
    }

    void RTApplication::Initialize()
    {
        // declear to make sure be initialzied firstly
        GetSingletonManager();
        systemInfo_ptr = &SystemInfo::Instance();
        windowsManager_ptr = &WindowsManager::Instance();
        windowsManager_ptr->Initialize();
    }
    void RTApplication::Run()
    {

    }
    void RTApplication::Tick()
    {
        windowsManager_ptr->Tick();
    }
}

// temporarily remove for.
// void* operator new(size_t uiSize)
// {
//     return ReiToEngine::RTApplication::Instance().GetMemoryManager().Allocate(uiSize, 8, false);
// }
// void* operator new[] (size_t uiSize)
// {
// 	return ReiToEngine::RTApplication::Instance().GetMemoryManager().Allocate(uiSize,8, true);
// }

// void operator delete (void* pvAddr) noexcept
// {
// 	return ReiToEngine::RTApplication::Instance().GetMemoryManager().Deallocate(pvAddr, 8, false);
// }

// void operator delete[] (void* pvAddr) noexcept
// {
// 	return ReiToEngine::RTApplication::Instance().GetMemoryManager().Deallocate(pvAddr, 8, true);
// }

// // 重载 malloc 和 free
// RT_FORCEINLINE void* malloc(size_t size)
// {
//     return ReiToEngine::RTApplication::Instance().GetMemoryManager().Allocate(size, 8, false);
// }

// RT_FORCEINLINE void free(void* ptr)
// {
//     return ReiToEngine::RTApplication::Instance().GetMemoryManager().Deallocate(ptr, 8, false);
// }
