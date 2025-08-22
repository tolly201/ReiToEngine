#include "RendererSystem.h"
namespace ReiToEngine {

RendererSystem::RendererSystem() = default;
RendererSystem::~RendererSystem() = default;

b8 RendererSystem::Initialize() {
    // Initialization logic for the renderer system
    RT_LOG_INFO("RendererSystem Initialized.");
    CreateRendererBackend(ERenderBackendType::VULKAN, "ReiToEngine", nullptr);
    CreateRendererFrontend(ERenderFrontendType::MESH, "ReiToEngine", nullptr);
    return true;
}

b8 RendererSystem::Tick(f64 delta_time) {
    SceneData scene_data;
    for (auto& pipeline : pipelines) {
        render_packet packet;
        RenderData data;
        if (!pipeline.frontend->BeginFrame(packet, data))
        {
            RT_LOG_ERROR("Failed to begin frame for frontend: ", static_cast<u8>(pipeline.frontend->GetType()));
            return false;
        }
        if (!pipeline.backend->BeginFrame(delta_time)) {
            return false;
        }

        if (!pipeline.frontend->EndFrame(packet, data)) {
            return false;
        }
        if (!pipeline.backend->EndFrame(delta_time)) {
            return false;
        }
    }
    return true;
}
}
