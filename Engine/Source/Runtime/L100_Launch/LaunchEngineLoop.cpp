#include "LaunchEngineLoop.h"
#include "L80_Function/L99_Application/RTApplication.h"

ReiToEngine::RTApplication ReiToEngine::RTApplication::instance_raw;
ReiToEngine::MemoryManager ReiToEngine::SingletonFactory::memoryManager;
ReiToEngine::RTApplication* ReiToEngine::RTApplication::instance_ptr = &ReiToEngine::RTApplication::instance_raw;
b8 CreateGameInstance(ReiToEngine::Game& game_instance)
{
    // Initialize the game instance with default values
    game_instance.app_config.start_pos_x = 100;
    game_instance.app_config.start_pos_y = 100;
    game_instance.app_config.start_height = 600;
    game_instance.app_config.start_width = 600;
    game_instance.app_config.name = "ReiToEngine Game";
    return true;
}

int RuntimeMainLoopEntry([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[])
{
    ReiToEngine::RTApplication::SetMemoryManager(nullptr, ReiToEngine::EMEMORY_MANAGER_TYPE::MIMALLOC);

    ReiToEngine::Game game_instance;
    RT_LOG_DEBUG_PLATFORM("start game instance");
    if (!CreateGameInstance(game_instance)) {
        RT_LOG_FATAL_PLATFORM("Failed to create game instance");
        return -1;
    }

    RT_LOG_DEBUG_PLATFORM("pass game instance");
    RT_LOG_DEBUG_PLATFORM("start application initialize");

    if (!ReiToEngine::RTApplication::Instance().Initialize(&game_instance)) {
        RT_LOG_FATAL_PLATFORM("Failed to initialize application");
        return -1;
    }

    RT_LOG_DEBUG_PLATFORM("start application start game");

    if (!ReiToEngine::RTApplication::Instance().StartGame()) {
        RT_LOG_FATAL_PLATFORM("Failed to start game");
        return -1;
    }

    RT_LOG_DEBUG_PLATFORM("start application main loop");

    if (!ReiToEngine::RTApplication::Instance().Run())
    {
        RT_LOG_FATAL_PLATFORM("Game Exit State Wrong");
        return -2;
    }

    ReiToEngine::RTApplication::Instance().Terminate();

    return 0;
}
