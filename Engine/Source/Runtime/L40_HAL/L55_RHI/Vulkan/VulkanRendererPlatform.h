#ifndef RHI_VULKAN_RENDERER_PLATFORM_H
#define RHI_VULKAN_RENDERER_PLATFORM_H
#include "vulkan/vulkan.h"
#include "L20_Platform/Include.h"
#include "../RendererTypes.h"
namespace ReiToEngine
{
void PlatformGetVulkanExtensions(ReiToEngine::List<const char*>& out_extensions);
b8 PlatformCreateVulkanSurface(RT_Platform_State& platform_state, SurfaceDesc& desc, VkInstance& instance, VkSurfaceKHR& out_surface);

b8 PlatformDestroyVulkanSurface(VkInstance& instance, VkSurfaceKHR& surface);
}

#endif
