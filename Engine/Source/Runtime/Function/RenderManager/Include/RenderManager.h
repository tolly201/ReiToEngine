#ifndef PLATFORM_FUNCTION_RENDER_MANAGER_H
#define PLATFORM_FUNCTION_RENDER_MANAGER_H

#include "Platform/RHI/Include/CPU/SoftRenderer.h"
#include "Platform/Singleton/RuntimeSingleton.h"
#include "Platform/Singleton/SingletonFactory.h"

namespace ReiToEngine {
class RenderManager : public Runtime_Singleton<RenderManager>
{
public:
    RenderManager() = default;
    ~RenderManager() = default;
    void Initialize() override {};
    void Terminate() override {};
    void Tick() override {};
    SoftRenderer softRenderer;
private:
};

}

#endif
