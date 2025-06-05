#include "Game.h"

#include "L20_Platform/Include.h"
b8 ReiToEngine::Game::Initialize(){
    return true;
}
b8 ReiToEngine::Game::LogicalTick(f64 delta_time){
    return true;
}
b8 ReiToEngine::Game::RenderTick(f64 delta_time){
    return true;
}
void ReiToEngine::Game::Terminate(){}
void  ReiToEngine::Game::OnResize(i16 width, i16 height){}
