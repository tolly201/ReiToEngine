#include "RendererSystem.h"
#include "Vulkan/VulkanRendererBackend.h"
namespace ReiToEngine {

RendererSystem::RendererSystem() = default;
RendererSystem::~RendererSystem() = default;

b8 RendererSystem::Initialize() {
    // Initialization logic for the renderer system
    RT_LOG_INFO_PLATFORM("RendererSystem Initialized.");
    CreateRendererBackend(ERenderBackendType::VULKAN, "ReiToEngine", nullptr);
    RT_LOG_INFO_PLATFORM("RendererSystem Frontend Initialized.");
    CreateRendererFrontend(ERenderFrontendType::MESH, "ReiToEngine", nullptr);
    RT_LOG_INFO_PLATFORM("RendererSystem Frontend Initialized.");

    pipelines.emplace_back();
    RendererPipeline& pipeline = pipelines.back();
    pipeline.backend = backends[ERenderBackendType::VULKAN];
    // pipeline.frontend = frontends[ERenderFrontendType::MESH];
    pipeline.frontend = nullptr;

    return true;
}

b8 RendererSystem::Tick(f64 delta_time) {
    [[maybe_unused]]SceneData scene_data;
    for (auto& pipeline : pipelines) {
        [[maybe_unused]]render_packet packet;
        [[maybe_unused]]RenderData data;

        // if (!pipeline.frontend->BeginFrame(packet, data))
        // {
        //     RT_LOG_ERROR_PLATFORM("Failed to begin frame for frontend.");
        //     return false;
        // }
        if (!pipeline.backend->BeginFrame(delta_time)) {
            return false;
        }

        pipeline.backend->UpdateGlobalState(
            Matrix4x4f::Identity(), Matrix4x4f::Identity(), Vec3f(0.0f), Vec4f(1.0f), 0);
        // if (!pipeline.frontend->EndFrame(packet, data)) {
        //     return false;
        // }
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
    RT_LOG_INFO_PLATFORM("RendererSystem Terminated.");
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
            RT_LOG_ERROR_PLATFORM("Unsupported renderer backend type: {}", static_cast<u32>(type));
            return false;
    }

    if (backend && backend->Initialize(type, application_name, plat_state)) {
        backends[type] = backend;
        RT_LOG_INFO_PLATFORM("Renderer backend of type", static_cast<u32>(type) ,"created and initialized.");
        return true;
    } else {
        RT_LOG_ERROR_PLATFORM("Failed to initialize renderer backend of type:", static_cast<u32>(type));
        delete backend;
        return false;
    }
}
}
