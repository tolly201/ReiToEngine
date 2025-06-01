#ifndef LAUNCHENGINELOOP_H
#define LAUNCHENGINELOOP_H
#include "Core/MinimalCore.h"
#include "Game.h"
extern b8 CreateGameInstance(Game& game_instance);

int RTENGINE_API RuntimeMainLoopEntry(int argc, const char* argv[]);
#endif
