#ifndef RUNTIME_LAUNCH_GAMETYPES_H
#define RUNTIME_LAUNCH_GAMETYPES_H
#include "Core/MinimalCore.h"
#include "Platform/Enums/RTApplicationEnums.h"

struct Game
{
    Game() = default;
    ~Game() = default;
    ReiToEngine::ApplicatonConfig app_config;
    b8 (*RT_GAME_Initialize)(Game& game_instance);
    b8 (*RT_GAME_LogicalTick)(Game& game_instance, f64 delta_time);
    b8 (*RT_GAME_RenderTick)(Game& game_instance, f64 delta_time);
    void (*RT_GAME_Terminate)(Game& game_instance);

    void (*RT_GAME_OnResize)(Game& game_instance, i16 width, i16 height);

    void* GameState;
};

#endif
