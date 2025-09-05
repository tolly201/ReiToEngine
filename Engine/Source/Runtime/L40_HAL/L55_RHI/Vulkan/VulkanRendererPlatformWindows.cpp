#include "L20_Platform/Include.h"
#ifdef RT_SYSTEM_WINDOWS
#include "VulkanRendererPlatform.h"
#include "vulkan/vulkan.h"
namespace ReiToEngine
{
void PlatformGetVulkanExtensions(ReiToEngine::List<const char*>& out_extensions)
{
}
};

b8 PlatformCreateVulkanSurface(RT_Platform_State& platform_state, SurfaceDesc& desc, VkInstance& instance, VkSurfaceKHR& out_surface)
{
    if (!desc.p_window)
    {
        RT_LOG_ERROR("PlatformCreateVulkanSurface: desc.p_window is null");
        return false;
    }

    VkWin32SurfaceCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    create_info.hwnd = static_cast<HWND>(desc.p_window->GetNativeHandle());
    create_info.hinstance = platform_state.hInstance;

    VkResult result = vkCreateWin32SurfaceKHR(instance, &create_info, nullptr, out_surface);
    if (result != VK_SUCCESS)
    {
        RT_LOG_ERROR("PlatformCreateVulkanSurface: vkCreateWin32SurfaceKHR failed with error code %d", result);
        return false;
    }

    return true;
}


#endif
