#include "LaunchEngineLoop.h"
#include "Core/Macro/Macro.h"
#include "Platform/Application/Include/RTApplication.h"
#include "Platform/Enums/RTApplicationEnums.h"
#include "./Game.h"

ReiToEngine::RTApplication ReiToEngine::RTApplication::instance_raw;
ReiToEngine::RTApplication* ReiToEngine::RTApplication::instance_ptr = &ReiToEngine::RTApplication::instance_raw;
b8 CreateGameInstance(Game& game_instance)
{
    // Initialize the game instance with default values
    game_instance.app_config.start_pos_x = 100;
    game_instance.app_config.start_pos_y = 100;
    game_instance.app_config.start_height = 600;
    game_instance.app_config.start_width = 800;
    game_instance.app_config.name = "ReiToEngine Game";
    return true;
}

int RuntimeMainLoopEntry(int argc, const char* argv[])
{
    RT_LOG_DEBUG("test");
    RT_LOG_ERROR("test");
    RT_LOG_FATAL("test");
    RT_LOG_INFO("test");
    RT_LOG_TRACE("test");
    RT_LOG_WARN("test");

    Game game_instance;
    RT_LOG_DEBUG("start game instance");
    if (!CreateGameInstance(game_instance)) {
        RT_LOG_FATAL("Failed to create game instance");
        return -1;
    }

    RT_LOG_DEBUG("pass game instance");
    RT_LOG_DEBUG("start application initialize");

    if (!ReiToEngine::RTApplication::Instance().Initialize(&game_instance)) {
        RT_LOG_FATAL("Failed to initialize application");
        return -1;
    }

    RT_LOG_DEBUG("start application start game");

    if (!ReiToEngine::RTApplication::Instance().StartGame()) {
        RT_LOG_FATAL("Failed to start game");
        return -1;
    }

    RT_LOG_DEBUG("start application main loop");

    if (!ReiToEngine::RTApplication::Instance().Run())
    {
        RT_LOG_FATAL("Game Exit State Wrong");
        return -2;
    }
    return 0;
}
