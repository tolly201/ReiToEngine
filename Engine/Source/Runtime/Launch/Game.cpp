#include "./Game.h"

#include "Core/Logger/Logger.h"
b8 Game::Initialize(){
    return true;
}
b8 Game::LogicalTick(f64 delta_time){
    return true;
}
b8 Game::RenderTick(f64 delta_time){
    return true;
}
void Game::Terminate(){}
void  Game::OnResize(i16 width, i16 height){}
