#include "RTApplication.h"
#include <functional>
#include <vector>
#include "L20_Platform/Include.h"

namespace ReiToEngine{
    b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context)
    {
        if (sender != RTApplication::Instance().GetApplicationState().main_window.window_ptr->GetInputMonitor())
        {
            RT_LOG_DEBUG("application_on_key: sender is not the main window input monitor.");
            return true;
        }
        if (code == static_cast<u16>(SYSTEM_EVENT_CODE::KEY_PRESS))
        {
            u8 key_code = context.data.u8[0];
            RT_LOG_INFO("catch key event", key_code);
            if (key_code == static_cast<u8>(KEY_CODE_KEYBOARD::ESCAPE))
            {
                RT_LOG_INFO("exit application");
                RTApplication::Instance().GetApplicationState().is_running = false;
            }

            if (key_code == static_cast<u8>(KEY_CODE_KEYBOARD::N))
            {
                RTApplication::Instance().CreateWindow();
                RT_LOG_INFO("create new window");
            }
        }

        return true;
    }

    RTApplication::RTApplication() = default;
    RTApplication::~RTApplication() = default;
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

        // InputSystem_ptr->Initialize();
        InitializeLog();

        RT_LOG_DEBUG("pass value finish");
        GetSingletonManager();
        RT_LOG_INFO("EventSystem size: %d", 123);
        RT_LOG_INFO("Hello %s, value=%f", "world", 3.14);
        RT_LOG_INFO("Hello %s, value=%f", "world", 3.14);
        RT_LOG_INFO("Hello, value=", "world", 3.14);
        // declear to make sure be initialzied firstly

        event_system_ptr = &EventSystem::Instance();
        input_system_ptr = &InputSystem::Instance();

        RT_LOG_INFO(sizeof(EventSystem));
        RT_LOG_INFO(sizeof(InputSystem));
        RT_LOG_INFO(alignof(EventSystem));
        RT_LOG_INFO(alignof(InputSystem));
        windowsManager_ptr = &WindowsManager::Instance();
        RT_LOG_DEBUG("window manager");
        //renderManager_ptr = &RenderManager::Instance();

        event_system_ptr->Initialize();
        input_system_ptr->Initialize();

        RT_LOG_DEBUG("render manager");
        windowsManager_ptr->Initialize();
        RT_LOG_DEBUG("win iniite manager");

        //renderManager_ptr->Initialize();
        RT_LOG_DEBUG("render init manager");

        initialized = true;
        return true;
    }

    b8 RTApplication::StartGame()
    {
        RT_LOG_DEBUG("APPLICATION STARTGAME");
        // if(!RT_Platform_Initialize(app_state.main_window, game_instance->app_config.name, game_instance->app_config.start_width, game_instance->app_config.start_height, game_instance->app_config.start_pos_x, game_instance->app_config.start_pos_y))
        if(!RT_Platform_Initialize())
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
        CreateMainWindow();

        event_system_ptr->RegisterEvent(static_cast<u32>(SYSTEM_EVENT_CODE::KEY_PRESS), this, application_on_key);

        app_state.is_running = true;
        app_state.is_paused = false;
        return true;
    }

    b8 RTApplication::Run()
    {
        RT_LOG_INFO(memoryManager.GetMemoryUsageReport());
        while(app_state.is_running)
        {
            if (!RT_Platform_PumpMessage())
            {
                app_state.is_running = false;
            }

            if (!app_state.is_paused)
            {
                if (!windowsManager_ptr->Tick(0.))
                {
                    RT_LOG_FATAL("WindowsManager tick failed.");
                    app_state.is_running = false;
                    break;
                }

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

                //input_system_ptr->Tick(0.);
            }
        }
        return true;
    }

    void RTApplication::Terminate()
    {
        ApplicationState& state = app_state;
        windowsManager_ptr->Terminate();
        game_instance->Terminate();
        event_system_ptr->UnregisterEvent(static_cast<u32>(SYSTEM_EVENT_CODE::KEY_PRESS), this, application_on_key);
        input_system_ptr->Terminate();
        event_system_ptr->Terminate();
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
