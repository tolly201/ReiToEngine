#ifndef RHI_VULKAN_RENDERER_PLATFORM_H
#define RHI_VULKAN_RENDERER_PLATFORM_H
#include "vulkan/vulkan.h"
#include "L20_Platform/Include.h"
#include "../RendererTypes.h"
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


    VkFormat depth_format;
    // mark logical device is initialized
    // recover logical device when all swapchains using this device are destroyed
    VkDevice logical_device;
    b8 is_inused;

    i32 find_memory_index(u32 type_filter, VkMemoryPropertyFlags properties);
};

struct VulkanSwapChainSupportInfo
{
    VkSurfaceCapabilities2KHR capabilities;
    List<VkSurfaceFormat2KHR> formats;
    List<VkPresentModeKHR> present_modes;
};

struct VulkanImage{
    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    VkFormat format;
    u32 width;
    u32 height;
    u32 depth;
    u32 mip_levels;
    u32 array_layers;
    VkSampleCountFlagBits samples;
    VkSharingMode sharing_mode;
};

struct VulkanSwapchainContext{
    VkSurfaceKHR surface;
    VulkanSwapChainSupportInfo swapchain_info;
    VulkanDeviceCombination* device_combination;
    Map<VulkanQueueFamilyIndicesType, i32> queue_family_indices;
    u32 width;
    u32 height;

    VkSurfaceFormat2KHR selected_surface_format;
    u8 max_frames_in_flight;
    VkSwapchainKHR swapchain;
    u32 image_count;
    List<VulkanImage> images;
    u32 current_image_index;
    u32 current_frame;
    b8 recreating_swapchain;

    List<VkSemaphore> image_available_semaphores;

    VulkanImage depth_image;
    VulkanPhysicalDeviceRequirements requirements;
};

struct VulkanContextRef{
    VkInstance& instance;
    VkAllocationCallbacks*& allocator;
};


void PlatformGetVulkanExtensions(ReiToEngine::List<const char*>& out_extensions);
b8 PlatformCreateVulkanSurface(RT_Platform_State& platform_state, SurfaceDesc& desc, VkInstance& instance, VkSurfaceKHR& out_surface);

b8 PlatformDestroyVulkanSurface(VkInstance& instance, VkSurfaceKHR& surface);

RT_FORCEINLINE b8 RT_VK_CHECK(VkResult expr) {
    RT_ASSERT(expr == VK_SUCCESS);
    return expr == VK_SUCCESS;
}

}

#endif
