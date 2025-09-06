#ifndef RHI_VULKAN_DEVICES_H
#define RHI_VULKAN_DEVICES_H
#include "vulkan/vulkan.h"
#include "L20_Platform/Include.h"
#include "../RendererTypes.h"
#include "VulkanRendererPlatform.h"

namespace ReiToEngine
{
enum class VulkanQueueFamilyIndicesType : u8{
    GRAPHICS = 0,
    PRESENT = 1,
    COMPUTE = 2,
    TRANSFER = 3,
    MAX = 4,
};

struct VulkanPhysicalDeviceRequirements{
    Map<VulkanQueueFamilyIndicesType, b8> queue_families;

    List<const char*> required_extensions;
    b8 sample_anisotropy;
    b8 discrete_gpu;
};

struct VulkanDeviceCombination{
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties2 device_properties;
    VkPhysicalDeviceFeatures2 device_features;
    VkPhysicalDeviceMemoryProperties2 memory_properties;
    List<VkQueueFamilyProperties2> queue_families;
    List<VkExtensionProperties> supported_extensions;

    Map<VulkanQueueFamilyIndicesType, List<i32>> available_queue_family_indices;

    Map<VulkanQueueFamilyIndicesType, List<i32>> dedicated_queue_family_indices;

    Map<VulkanQueueFamilyIndicesType, List<i32>> inuse_queue_family_indices;

    // mark logical device is initialized
    // recover logical device when all swapchains using this device are destroyed
    VkDevice logical_device;
    b8 is_inused;
};

struct VulkanSwapChainSupportInfo
{
    VkSurfaceCapabilities2KHR capabilities;
    List<VkSurfaceFormat2KHR> formats;
    List<VkPresentModeKHR> present_modes;
};

struct VulkanSwapchainContext{
    VkSurfaceKHR surface;
    VulkanSwapChainSupportInfo swapchain_info;
    VulkanDeviceCombination* device_combination;

    Map<VulkanQueueFamilyIndicesType, i32> queue_family_indices;

    VulkanPhysicalDeviceRequirements requirements;
};

b8 vulkan_initalize_physical_devices(VkInstance& instance, List<VulkanDeviceCombination>& out_physical_devices);

b8 vulkan_physical_device_select(VkInstance& instance, VulkanSwapchainContext& swapchain_content, List<VulkanDeviceCombination>& physical_devices);
b8 vulkan_logical_device_create(VulkanSwapchainContext& swapchain_context);
b8 vulkan_physical_device_destroy();
b8 vulkan_logical_device_destroy(VulkanDeviceCombination& dc);

b8 platform_get_required_vulkan_extensions(List<const char*>&);
}

#endif
