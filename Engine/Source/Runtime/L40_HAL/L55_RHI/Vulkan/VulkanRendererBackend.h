#ifndef RHI_VULKAN_RENDERER_BACKEND_H
#define RHI_VULKAN_RENDERER_BACKEND_H
#include "../IRendererBackend.h"
#include "vulkan/vulkan.h"
#include "VulkanRendererPlatform.h"
namespace ReiToEngine
{
class VulkanRenderBackend : public IRendererBackend {
public:
    VulkanRenderBackend() = default;
    ~VulkanRenderBackend() = default;

    b8 Initialize(ERenderBackendType renderer_type, const char* application_name, PlatformState* plat_state) override;
    b8 Terminate() override;
    b8 Tick() override;
    b8 Resized(u32 width, u32 height) override;

    b8 BeginFrame(f64 delta_time) override;
    b8 EndFrame(f64 delta_time) override;
private:
    VkInstance instance;
    VkAllocationCallbacks* allocator;

    b8 debugger_enabled;
    VkDebugUtilsMessengerEXT debug_messenger;

    VkPhysicalDevice physical_device;
    VkDevice logical_device;

};

}


#endif
