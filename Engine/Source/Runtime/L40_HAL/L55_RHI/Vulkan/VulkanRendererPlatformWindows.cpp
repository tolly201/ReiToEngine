#include "L20_Platform/Include.h"
#ifdef RT_SYSTEM_WINDOWS
#include "VulkanRendererPlatform.h"
#include "../RendererTypes.h"
#include "vulkan/vulkan.h"
#include <vulkan/vulkan_win32.h>
#include "VulkanDevices.h"
namespace ReiToEngine
{
void PlatformGetVulkanExtensions(ReiToEngine::List<const char*>& out_extensions)
{
    out_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
}

b8 PlatformCreateVulkanSurface(RT_Platform_State& platform_state, SurfaceDesc& desc, VkInstance& instance, VkSurfaceKHR& out_surface)
{
    if (!desc.p_window)
    {
        RT_LOG_ERROR("PlatformCreateVulkanSurface: desc.p_window is null");
        return false;
    }

    VkWin32SurfaceCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    create_info.hwnd = static_cast<HWND>(desc.p_window->GetNativeWindowHandle());
    create_info.hinstance = platform_state.hInstance;

    VkResult result = vkCreateWin32SurfaceKHR(instance, &create_info, nullptr, &out_surface);
    if (result != VK_SUCCESS)
    {
        RT_LOG_ERROR("PlatformCreateVulkanSurface: vkCreateWin32SurfaceKHR failed with error code %d", result);
        return false;
    }

    return true;
}

b8 PlatformDestroyVulkanSurface(VkInstance& instance, VkSurfaceKHR& surface)
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
    RT_LOG_INFO("PlatformDestroyVulkanSurface: Vulkan surface destroyed successfully");
    return true;
};

b8 platform_get_required_vulkan_extensions(List<const char*>& out_extensions){
    return true;
}
}
#endif
