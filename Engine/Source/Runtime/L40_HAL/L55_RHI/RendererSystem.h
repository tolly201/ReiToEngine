#ifndef RHI_RENDERER_SYSTEM_H
#define RHI_RENDERER_SYSTEM_H

#include "L0_Macro/Include.h"
#include "L20_Platform/Include.h"
#include "IRendererFrontend.h"
namespace ReiToEngine {

struct RendererPipelineDesc{

};

struct RendererPipeline {
    RendererPipelineDesc desc;
    IRendererBackend* backend;
    IRendererFrontend* frontend;
};

class RendererSystem : public Runtime_Singleton<RendererSystem> {
    public:
    RendererSystem();
    ~RendererSystem();

    b8 Initialize() override;
    b8 Tick(f64) override;
    b8 Terminate() override;

    b8 CreateRendererBackend(ERenderBackendType type, const char* application_name, PlatformState* plat_state);
    b8 CreateRendererFrontend([[maybe_unused]] ERenderFrontendType type, [[maybe_unused]] const char* application_name, [[maybe_unused]] PlatformState* plat_state){
        return true;
    };

    b8 CreateRendererPipeline(const RendererPipelineDesc&& desc, ERenderBackendType backend_type, ERenderFrontendType frontend_type);

    b8 CreateWindowsSurface(RT_Platform_State& platform_state, SurfaceDesc&& desc)
    {
        auto it = backends.find(ERenderBackendType::VULKAN);
        if (it == backends.end())
        {
        RT_LOG_DEBUG_FMT_PLATFORM("width: {}, height: {}", desc.width, desc.height);
            RT_LOG_ERROR_PLATFORM("CreateWindowsSurface: Vulkan backend not found");
            return false;
        }
        return it->second->CreateSurface(platform_state, desc);
    }
private:
    ReiToEngine::Map<ERenderBackendType, IRendererBackend*> backends;
    ReiToEngine::Map<ERenderFrontendType, IRendererBackend*> frontends;
    ReiToEngine::List<RendererPipeline> pipelines;
};
}
#endif
