#include "../Include/RTApplication.h"
#include <functional>
#include <vector>
#include "Function/RenderManager/Include/RenderManager.h"
#include "Core/HAL/Input/Include/InputEnums.h"
#include "Platform/WindowsManager/Include/WindowsManager.h"
#include "Core/Logger/Logger.h"
#include "Core/HAL/PlatformDefines.h"

namespace ReiToEngine{
    RTApplication::RTApplication() = default;
    RTApplication::~RTApplication() = default;
    RTCMemoryManager& RTApplication::GetMemoryManager()
    {
        return RTApplication::memoryManager;
    }

    RTApplication& RTApplication::Instance()
    {
        return *instance_ptr;
    }

    b8 RTApplication::Initialize(IGame* game_instance)
    {
        if (initialized)
        {
            RT_LOG_FATAL("Application already initialized");
            return false;
        }
        this->game_instance = game_instance;

        app_state.height = game_instance->app_config.start_height;
        app_state.width = game_instance->app_config.start_width;
        app_state.pos_x = game_instance->app_config.start_pos_x;
        app_state.pos_y = game_instance->app_config.start_pos_y;

        app_state.is_paused = false;
        app_state.is_running = false;
        app_state.last_time = 0.;

        // inputSystem_ptr->Initialize();
        InitializeLog();

        RT_LOG_DEBUG("pass value finish");
        GetSingletonManager();
        RT_LOG_DEBUG("singleton manager");
        // declear to make sure be initialzied firstly
        windowsManager_ptr = &WindowsManager::Instance();
        RT_LOG_DEBUG("window manager");
        // inputSystem_ptr = &InputSystem::Instance();
        renderManager_ptr = &RenderManager::Instance();
        RT_LOG_DEBUG("render manager");
        windowsManager_ptr->Initialize();
        RT_LOG_DEBUG("win iniite manager");

        renderManager_ptr->Initialize();
        RT_LOG_DEBUG("render init manager");

        initialized = true;
        return true;
    }

    b8 RTApplication::StartGame()
    {
        RT_LOG_DEBUG("APPLICATION STARTGAME");
        if(!RT_HAL_Initialize(app_state.main_window, game_instance->app_config.name, game_instance->app_config.start_width, game_instance->app_config.start_height, game_instance->app_config.start_pos_x, game_instance->app_config.start_pos_y))
        {
            RT_LOG_FATAL("Failed to START CREATE PLATFORM WINDOW.");
            return false;
        };

        RT_LOG_DEBUG("APPLICATION Game Initialize");

        if (!game_instance->Initialize())
        {
            RT_LOG_FATAL("Failed to initialize game instance.");
            return false;
        }
        RT_LOG_DEBUG("APPLICATION Game Initialize");

        game_instance->OnResize(app_state.width, app_state.height);

        RT_LOG_DEBUG("APPLICATION Game Initialize");

        printf("base run\n");
        app_state.is_running = true;
        app_state.is_paused = false;
        return true;
    }

    b8 RTApplication::Run()
    {
        RT_LOG_INFO(memoryManager.GetMemoryUsageReport());
        while(app_state.is_running)
        {
            if (!RT_HAL_PumpMessage(app_state.main_window))
            {
                app_state.is_running = false;
            }

            if (!app_state.is_paused)
            {
                if (!game_instance->LogicalTick(0.))
                {
                    RT_LOG_FATAL("Game logical tick failed.");
                    app_state.is_running = false;
                    break;
                }
                if (!game_instance->RenderTick(0.))
                {
                    RT_LOG_FATAL("Game render tick failed.");
                    app_state.is_running = false;
                    break;
                }
            }
        }
        return true;
    }

    void RTApplication::Terminate()
    {
        ApplicationState& state = app_state;
        windowsManager_ptr->Terminate();
        game_instance->Terminate();
    }

    const ApplicationState& RTApplication::GetConstApplicationState()
    {
        return app_state;
    }

    ApplicationState& RTApplication::GetApplicationState()
    {
        return app_state;
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
