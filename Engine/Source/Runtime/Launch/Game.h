#ifndef RUNTIME_LAUNCH_GAME_H
#define RUNTIME_LAUNCH_GAME_H
#include "./GameTypes.h"

class Game : public IGame
{
public:
    Game() = default;
    ~Game() = default;
    b8 Initialize() override;
    b8 LogicalTick(f64 delta_time) override;
    b8 RenderTick(f64 delta_time) override;
    void Terminate() override;
    void OnResize(i16 width, i16 height) override;
};


// b8 RT_GAME_Initialize(Game& game_instance);
// b8 RT_GAME_LogicalTick(Game& game_instance, f64 delta_time);
// b8 RT_GAME_RenderTick(Game& game_instance, f64 delta_time);
// void RT_GAME_Terminate(Game& game_instance);
// void RT_GAME_OnResize(Game& game_instance, i16 width, i16 height);

#endif
