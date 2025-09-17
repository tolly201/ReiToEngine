#ifndef RHI_VULKAN_RENDERER_BACKEND_H
#define RHI_VULKAN_RENDERER_BACKEND_H
#include "../IRendererBackend.h"
#include "vulkan/vulkan.h"
#include "VulkanRendererPlatform.h"
#include "L20_Platform/Include.h"
#include "VulkanExtensions.h"
#include "VulkanDevices.h"
#include "VulkanDefines.h"
namespace ReiToEngine
{
class VulkanRenderBackend : public IRendererBackend {
public:
    VulkanRenderBackend() = default;
    ~VulkanRenderBackend() = default;

    b8 Initialize(ERenderBackendType renderer_type, const char* application_name, PlatformState* plat_state) override;
    b8 Terminate() override;
    b8 Tick() override;
    b8 Resized(SurfaceDesc& desc, u32 width, u32 height) override;

    b8 BeginFrame(f64 delta_time) override;
    b8 EndFrame(f64 delta_time) override;

    b8 CreateSurface(RT_Platform_State& platform_state, SurfaceDesc& desc) override;

private:
    VkInstance instance;
    VkAllocationCallbacks* allocator;

    b8 debugger_enabled;
    VkDebugUtilsMessengerEXT debug_messenger;

    List<VulkanDeviceCombination> devices;

    List<VulkanSwapchainContext> swapchains;
    Map<IWindow*, VulkanSwapchainContext*> swapchain_map;
};

}


#endif
