#include "LaunchEngineLoop.h"
#include "Core/Macro/Macro.h"
#include "Platform/Application/Include/RTApplication.h"
#include "Platform/Enums/RTApplicationEnums.h"
#include "./Game.h"
#ifdef RT_SYSTEM_WINDOWS
#include "Platform/Application/Include/RTWindowsApplication.h"
ReiToEngine::RTWindowsApplication ReiToEngine::RTWindowsApplication::instance;

ReiToEngine::RTApplication* ReiToEngine::RTApplication::instance_ptr = &ReiToEngine::RTWindowsApplication::instance;

#elif defined(RT_SYSTEM_APPLE) // macOS
#include "Platform/Application/Include/RTMacApplication.h"

ReiToEngine::RTMacApplication ReiToEngine::RTMacApplication::instance;

ReiToEngine::RTApplication* ReiToEngine::RTApplication::instance_ptr = &ReiToEngine::RTMacApplication::instance;

#elif defined(RT_SYSTEM_LINUX) // Linux

#else
#endif

b8 CreateGameInstance(Game& game_instance)
{
    // Initialize the game instance with default values
    game_instance.app_config.start_pos_x = 100;
    game_instance.app_config.start_pos_y = 100;
    game_instance.app_config.start_height = 600;
    game_instance.app_config.start_width = 800;
    game_instance.app_config.name = "ReiToEngine Game";

    game_instance.RT_GAME_Initialize = RT_GAME_Initialize;
    game_instance.RT_GAME_LogicalTick = RT_GAME_LogicalTick;
    game_instance.RT_GAME_RenderTick = RT_GAME_RenderTick;
    game_instance.RT_GAME_Terminate = RT_GAME_Terminate;
    game_instance.RT_GAME_OnResize = RT_GAME_OnResize;

    game_instance.GameState = nullptr;
    return true;
}

int RuntimeMainLoopEntry(int argc, const char* argv[])
{
    Game game_instance;
    if (!CreateGameInstance(game_instance)) {
        RTFATAL("Failed to create game instance");
        return -1;
    }

    if (
        !game_instance.RT_GAME_Initialize ||
        !game_instance.RT_GAME_LogicalTick ||
        !game_instance.RT_GAME_RenderTick ||
        !game_instance.RT_GAME_Terminate ||
        !game_instance.RT_GAME_OnResize
    )
    {
        RTFATAL("Game instance functions are not properly set");
        return -1;
    }

    if (!ReiToEngine::RTApplication::Instance().Initialize(game_instance)) {
        RTFATAL("Failed to initialize application");
        return -1;
    }

    if (!ReiToEngine::RTApplication::Instance().StartGame()) {
        RTFATAL("Failed to start game");
        return -1;
    }

    if (!ReiToEngine::RTApplication::Instance().Run())
    {
        RTFATAL("Game Exit State Wrong");
        return -2;
    }
    return 0;
}
