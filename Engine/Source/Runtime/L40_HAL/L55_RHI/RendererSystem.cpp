#include "RendererSystem.h"
#include "Vulkan/VulkanRendererBackend.h"
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
            RT_LOG_ERROR("Failed to begin frame for frontend.");
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

b8 RendererSystem::Terminate() {
    for (auto& [type, backend] : backends) {
        if (backend) {
            backend->Terminate();
            delete backend;
        }
    }
    backends.clear();

    for (auto& [type, frontend] : frontends) {
        if (frontend) {
            // Assuming frontends have a Terminate method
            // frontend->Terminate();
            delete frontend;
        }
    }
    frontends.clear();
    pipelines.clear();
    RT_LOG_INFO("RendererSystem Terminated.");
    return true;
}

b8 RendererSystem::CreateRendererBackend(ERenderBackendType type, const char* application_name, PlatformState* plat_state) {
    IRendererBackend* backend = nullptr;
    switch (type) {
        case ERenderBackendType::VULKAN:
            backend = new VulkanRenderBackend();
            break;
        // Add cases for other backend types as needed
        default:
            RT_LOG_ERROR("Unsupported renderer backend type: {}", static_cast<u32>(type));
            return false;
    }

    if (backend && backend->Initialize(type, application_name, plat_state)) {
        backends[type] = backend;
        RT_LOG_INFO("Renderer backend of type", static_cast<u32>(type) ,"created and initialized.");
        return true;
    } else {
        RT_LOG_ERROR("Failed to initialize renderer backend of type:", static_cast<u32>(type));
        delete backend;
        return false;
    }
}
}
