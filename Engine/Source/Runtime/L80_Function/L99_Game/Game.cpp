#include "Game.h"

#include "L20_Platform/Include.h"
b8 ReiToEngine::Game::Initialize(){
    return true;
}
b8 ReiToEngine::Game::LogicalTick([[maybe_unused]]f64 delta_time){
    return true;
}
b8 ReiToEngine::Game::RenderTick([[maybe_unused]]f64 delta_time){
    return true;
}
void ReiToEngine::Game::Terminate(){}
void  ReiToEngine::Game::OnResize([[maybe_unused]]i16 width,[[maybe_unused]] i16 height){}
