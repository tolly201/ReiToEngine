#ifndef RHI_VULKAN_COMMAND_BUFFER_H
#define RHI_VULKAN_COMMAND_BUFFER_H
#include "vulkan/vulkan.h"
#include "L20_Platform/Include.h"
#include "../RendererTypes.h"
#include "VulkanRendererPlatform.h"

namespace ReiToEngine
{
b8 vulkan_initalize_physical_devices(VkInstance& instance, List<VulkanDeviceCombination>& out_physical_devices);

b8 vulkan_physical_device_select(VkInstance& instance, VulkanSwapchainContext& swapchain_content, List<VulkanDeviceCombination>& physical_devices);
b8 vulkan_logical_device_create(VulkanSwapchainContext& swapchain_context);
b8 vulkan_physical_device_destroy();
b8 vulkan_logical_device_destroy(VulkanDeviceCombination& dc);

b8 platform_get_required_vulkan_extensions(List<const char*>&);
b8 vulkan_device_query_swapchain_support(VkPhysicalDevice& device, VkSurfaceKHR& surface, VulkanSwapChainSupportInfo& out_info);

b8 vulkan_device_detect_depth_format(VulkanDeviceCombination& device_combination);
}


#endif
