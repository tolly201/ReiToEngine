#ifndef RUNTIME_LAUNCH_GAME_H
#define RUNTIME_LAUNCH_GAME_H
#include "./GameTypes.h"
b8 RT_GAME_Initialize(Game& game_instance);
b8 RT_GAME_LogicalTick(Game& game_instance, f64 delta_time);
b8 RT_GAME_RenderTick(Game& game_instance, f64 delta_time);
void RT_GAME_Terminate(Game& game_instance);
void RT_GAME_OnResize(Game& game_instance, i16 width, i16 height);

#endif
