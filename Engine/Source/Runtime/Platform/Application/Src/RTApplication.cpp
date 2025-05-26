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
    RTDebugHeapMemoryManager& RTApplication::GetMemoryManager()
    {
        return RTApplication::memoryManager;
    }

    RTApplication& RTApplication::Instance()
    {
        return *instance_ptr;
    }

    b8 RTApplication::Initialize(Game& game_instance)
    {
        if (initialized)
        {
            RTFATAL("Application already initialized");
            return false;
        }
        this->game_instance = game_instance;

        app_state.height = game_instance.app_config.start_height;
        app_state.width = game_instance.app_config.start_width;
        app_state.pos_x = game_instance.app_config.start_pos_x;
        app_state.pos_y = game_instance.app_config.start_pos_y;

        app_state.is_paused = false;
        app_state.is_running = false;
        app_state.last_time = 0.;

        // inputSystem_ptr->Initialize();
        InitializeLog();
        RTFATAL("test");
        RTERROR("test");
        RTDEBUG("test");
        RTTRACE("test");
        RTINFO("test");
        RTWARN("test");

        GetSingletonManager();
        // declear to make sure be initialzied firstly
        windowsManager_ptr = &WindowsManager::Instance();
        // inputSystem_ptr = &InputSystem::Instance();
        renderManager_ptr = &RenderManager::Instance();

        windowsManager_ptr->Initialize();
        renderManager_ptr->Initialize();

        initialized = true;
        return true;
    }
    b8 RTApplication::StartGame()
    {
        if(!RT_HAL_Initialize(app_state.main_window, game_instance.app_config.name, game_instance.app_config.start_width, game_instance.app_config.start_height, game_instance.app_config.start_pos_x, game_instance.app_config.start_pos_y))
        {
            RTFATAL("Failed to START CREATE PLATFORM WINDOW.");
            return false;
        };

        if (!game_instance.RT_GAME_Initialize(game_instance))
        {
            RTFATAL("Failed to initialize game instance.");
            return false;
        }
        game_instance.RT_GAME_OnResize(game_instance, app_state.width, app_state.height);

        printf("base run\n");
        app_state.is_running = true;
        app_state.is_paused = false;
        return true;
    }

    b8 RTApplication::Run()
    {
        while(app_state.is_running)
        {
            if (!RT_HAL_PumpMessage(app_state.main_window))
            {
                app_state.is_running = false;
            }

            if (!app_state.is_paused)
            {
                if (!game_instance.RT_GAME_LogicalTick(game_instance, 0.))
                {
                    RTFATAL("Game logical tick failed.");
                    app_state.is_running = false;
                    break;
                }
                {
                    RTFATAL("Game logical tick failed.");
                    app_state.is_running = false;
                    break;
                }
                if (!game_instance.RT_GAME_RenderTick(game_instance, 0.))
                {
                    RTFATAL("Game render tick failed.");
                    app_state.is_running = false;
                    break;
                }
            }
        }
    }

    void RTApplication::Terminate()
    {
        ApplicationState& state = app_state;
        windowsManager_ptr->Terminate();
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
