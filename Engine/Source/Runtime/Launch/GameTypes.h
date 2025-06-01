#ifndef RUNTIME_LAUNCH_GAMETYPES_H
#define RUNTIME_LAUNCH_GAMETYPES_H
#include "Core/MinimalCore.h"
#include "Platform/Enums/RTApplicationEnums.h"
class IGame
{
public:
    IGame() = default;
    ~IGame() = default;
    ReiToEngine::ApplicatonConfig app_config;
    void* GameState;

    virtual b8 Initialize() = 0;
    virtual b8 LogicalTick(f64 delta_time) = 0;
    virtual b8 RenderTick(f64 delta_time) = 0;
    virtual void Terminate() = 0;
    virtual void OnResize(i16 width, i16 height) = 0;

};

#endif
