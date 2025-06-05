#ifndef LAUNCHENGINELOOP_H
#define LAUNCHENGINELOOP_H
#include "L80_Function/L99_Game/Game.h"
extern b8 CreateGameInstance(ReiToEngine::Game& game_instance);

int RTENGINE_API RuntimeMainLoopEntry(int argc, const char* argv[]);
#endif
