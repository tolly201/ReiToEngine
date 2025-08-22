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
    IRendererBackEnd* backend;
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
    b8 CreateRendererFrontend(ERenderFrontendType type, const char* application_name, PlatformState* plat_state);

    b8 CreateRendererPipeline(const RendererPipelineDesc&& desc, ERenderBackendType backend_type, ERenderFrontendType frontend_type);
private:
    ReiToEngine::Map<ERenderBackendType, IRendererBackEnd*> backends;
    ReiToEngine::Map<ERenderFrontendType, IRendererBackEnd*> frontends;
    ReiToEngine::List<RendererPipeline> pipelines;
};
}
#endif
