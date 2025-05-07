#include "../Include/RTApplication.h"
#include <functional>
#include <vector>
#include "Function/RenderManager/Include/RenderManager.h"
#include "Platform/HAL/Input/Include/InputEnums.h"
#include "Platform/HAL/System/Include/SystemInfo.h"
#include "Platform/WindowsManager/Include/WindowsManager.h"
#include "test.h"
#include "Core/Logger/Logger.h"

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

    void RTApplication::Initialize(ApplicatonConfig& config)
    {
        app_config = config;
        app_state.height = config.start_height;
        app_state.width = config.start_width;
        app_state.pos_x = config.start_pos_x;
        app_state.pos_y = config.start_pos_y;

        app_state.is_paused = false;
        app_state.is_running = false;
        app_state.last_time = 0.;

        // declear to make sure be initialzied firstly
        GetSingletonManager();
        systemInfo_ptr = &SystemInfo::Instance();
        windowsManager_ptr = &WindowsManager::Instance();
        // inputSystem_ptr = &InputSystem::Instance();
        renderManager_ptr = &RenderManager::Instance();


        // inputSystem_ptr->Initialize();
        windowsManager_ptr->Initialize();
        renderManager_ptr->Initialize();

        printf("base init\n");
        test::InitTestSpace(&renderManager_ptr->softRenderer);
        InitializeLog();
    }
    void RTApplication::Run()
    {
        printf("base run\n");
        // inputSystem_ptr->AddInputCallback(EINPUT_EVENT_TYPE::EVENT_KEY_PRESS, test::MoveCamera);
        // inputSystem_ptr->AddInputCallback(EINPUT_EVENT_TYPE::EVENT_POINTER_MOVE, test::MoveCameraMouse);
        app_state.is_running = true;
        app_state.is_paused = false;
        windowsManager_ptr->RTCreateWindow(app_state.width, app_state.height, 3);
    }

    void RTApplication::Tick()
    {
        // inputSystem_ptr->Tick();
        // std::vector<InputEvent> events = inputSystem_ptr->GetInputEvents();
        // std::vector<systemEvent> events = inputSystem_ptr->GetInputEvents();
        // Consoles_ptr->Tick();
    }

    void RTApplication::Terminate()
    {
        windowsManager_ptr->Terminate();
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
