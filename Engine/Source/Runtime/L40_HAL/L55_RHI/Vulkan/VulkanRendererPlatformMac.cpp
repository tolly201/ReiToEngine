#include "L20_Platform/Include.h"
#ifdef RT_SYSTEM_APPLE
#include "VulkanRendererPlatform.h"
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_metal.h"
#include "L20_Platform/L37_Window/MACOSX/MacOSXWindow.h"
#import <Cocoa/Cocoa.h>
namespace ReiToEngine
{
void PlatformGetVulkanExtensions(ReiToEngine::List<const char*>& out_extensions)
{
    out_extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
    out_extensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
}

b8 PlatformCreateVulkanSurface(RT_Platform_State& platform_state, SurfaceDesc& desc, VkInstance& instance, VkSurfaceKHR& out_surface)
{
    if (!desc.p_window)
    {
        RT_LOG_ERROR("PlatformCreateVulkanSurface: desc.p_window is null");
        return false;
    }

    VkMetalSurfaceCreateInfoEXT create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    create_info.pNext = nullptr;
    create_info.flags = 0;
    create_info.pLayer = static_cast<CAMetalLayer*>(
        static_cast<MacOSXWindow*>(desc.p_window)->cocoaView->GetViewInstance().layer);

    RT_VK_CHECK(vkCreateMetalSurfaceEXT(instance, &create_info, nullptr, &out_surface));
    RT_LOG_INFO("PlatformCreateVulkanSurface: Vulkan surface created successfully");
    return true;
};

b8 PlatformDestroyVulkanSurface(VkInstance& instance, VkSurfaceKHR& surface)
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
    RT_LOG_INFO("PlatformDestroyVulkanSurface: Vulkan surface destroyed successfully");
    return true;
}

b8 platform_get_required_vulkan_extensions(List<const char*>& out_extensions){
    out_extensions.push_back("VK_KHR_portability_subset");
    return true;
}
}
#endif
