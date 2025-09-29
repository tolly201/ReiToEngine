
#include "VulkanSwapChain.h"
#include "VulkanDevices.h"
#include "VulkanImage.h"
namespace ReiToEngine
{
void create(VulkanContextRef context, VulkanSwapchainContext& swapchain_context, u32 width, u32 height);
void destroy(VulkanContextRef context, VulkanSwapchainContext& swapchain_context);

void vulkan_swapchain_create(VulkanContextRef context, VulkanSwapchainContext& swapchain_context)
{
    create(context, swapchain_context, swapchain_context.width, swapchain_context.height);
}

b8 vulkan_swapchain_recreate(VulkanContextRef context, VulkanSwapchainContext& swapchain_context)
{
    destroy(context, swapchain_context);
    create(context, swapchain_context, swapchain_context.width, swapchain_context.height);
    return true;
}

void vulkan_swapchain_destroy(VulkanContextRef context, VulkanSwapchainContext& swapchain_context)
{
    destroy(context, swapchain_context);
}

b8 vulkan_swapchain_acquire_next_image_index([[maybe_unused]]VulkanContextRef context ,VulkanSwapchainContext& swapchain_context, u32 timeout_us, VkSemaphore& semaphore, VkFence* fence,[[maybe_unused]] u32& image_index)
{
    // 检查 device_combination 是否有效
    if (!swapchain_context.device_combination) {
        RT_LOG_FATAL("swapchain_context.device_combination is nullptr!");
        return false;
    }

    // 检查信号量和 fence 是否有效
    if (semaphore == VK_NULL_HANDLE) {
        RT_LOG_FATAL("semaphore is VK_NULL_HANDLE!");
        return false;
    }
    if (fence && *fence == VK_NULL_HANDLE) {
        RT_LOG_FATAL("fence is VK_NULL_HANDLE!");
        return false;
    }

    // 检查 current_frame 是否越界
    if (swapchain_context.current_frame >= swapchain_context.image_available_semaphores.size()) {
        RT_LOG_FATAL("current_frame out of range!");
        return false;
    }

    // Perform acquire
    VkResult result = vkAcquireNextImageKHR(
        swapchain_context.device_combination->logical_device, swapchain_context.swapchain, timeout_us, semaphore, fence ? *fence : VK_NULL_HANDLE, &swapchain_context.current_image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        // Defer actual rebuild to the main loop to ensure framebuffers/command buffers are recreated too.
        swapchain_context.recreating_swapchain = true;
        return false;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        RT_LOG_ERROR("Failed to acquire swap chain image!");
        return false;
    }
    return true;
}
b8 vulkan_swapchain_present(VulkanContextRef context ,VulkanSwapchainContext& swapchain_context,[[maybe_unused]] VkQueue graphics_queue, VkQueue present_queue, VkSemaphore complete_semaphore, u32 present_image_index)
{
    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &complete_semaphore;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain_context.swapchain;
    present_info.pImageIndices = &present_image_index;
    present_info.pResults = nullptr;

    VkResult result = vkQueuePresentKHR(present_queue, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        vulkan_swapchain_recreate(context, swapchain_context);
        return false;
    }
    else if (result != VK_SUCCESS)
    {
        RT_LOG_FATAL("Failed to present swap chain image!");
        return false;
    }

    swapchain_context.current_frame = (swapchain_context.current_frame + 1) % swapchain_context.max_frames_in_flight;
    return true;
}

void create(VulkanContextRef context, VulkanSwapchainContext& swapchain_context,[[maybe_unused]] u32 width,[[maybe_unused]] u32 height)
{
    VkExtent2D extent = { swapchain_context.width, swapchain_context.height };

    b8 found = false;
    for (u32 i = 0; i < swapchain_context.swapchain_info.formats.size(); ++i) {
        const VkSurfaceFormat2KHR& available_format = swapchain_context.swapchain_info.formats[i];
        if (available_format.surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            available_format.surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            swapchain_context.selected_surface_format = available_format;
            found = true;
            break;
        }
    }

    if (!found) {
        swapchain_context.selected_surface_format = swapchain_context.swapchain_info.formats[0];
    }

    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (u32 i = 0; i < swapchain_context.swapchain_info.present_modes.size(); ++i) {
        if (swapchain_context.swapchain_info.present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        else if (swapchain_context.swapchain_info.present_modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    //requery swapchain support
    //vulkan_device_query_swapchain_support(swapchain_context.device_combination->physical_device, swapchain_context.surface, swapchain_context.swapchain_info);

    if(swapchain_context.swapchain_info.capabilities.surfaceCapabilities.currentExtent.width != UINT32_MAX)
    {
        extent = swapchain_context.swapchain_info.capabilities.surfaceCapabilities.currentExtent;
    }

    VkExtent2D min = swapchain_context.swapchain_info.capabilities.surfaceCapabilities.minImageExtent;
    VkExtent2D max = swapchain_context.swapchain_info.capabilities.surfaceCapabilities.maxImageExtent;

    extent.width = RTCLAMP(extent.width, min.width, max.width);
    extent.height = RTCLAMP(extent.height, min.height, max.height);

    u32 image_count = swapchain_context.swapchain_info.capabilities.surfaceCapabilities.minImageCount + 1;

    if (swapchain_context.swapchain_info.capabilities.surfaceCapabilities.maxImageCount > 0 && image_count > swapchain_context.swapchain_info.capabilities.surfaceCapabilities.maxImageCount) {
        image_count = swapchain_context.swapchain_info.capabilities.surfaceCapabilities.maxImageCount;
    }

    swapchain_context.max_frames_in_flight = image_count - 1;

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = swapchain_context.surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = swapchain_context.selected_surface_format.surfaceFormat.format;
    create_info.imageColorSpace = swapchain_context.selected_surface_format.surfaceFormat.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::GRAPHICS] != swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::PRESENT]) {
        u32 queue_family_indices[] = {
            (u32)swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::GRAPHICS],
            (u32)swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::PRESENT]
        };
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = swapchain_context.swapchain_info.capabilities.surfaceCapabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    RT_VK_CHECK(vkCreateSwapchainKHR(swapchain_context.device_combination->logical_device, &create_info, nullptr, &swapchain_context.swapchain));

    swapchain_context.image_count = 0;
    swapchain_context.current_frame = 0;
    swapchain_context.current_image_index = 0;
    RT_VK_CHECK(vkGetSwapchainImagesKHR(swapchain_context.device_combination->logical_device, swapchain_context.swapchain, &swapchain_context.image_count, nullptr));

    swapchain_context.images.clear();

    List<VkImage> vk_images;
    vk_images.resize(swapchain_context.image_count);

    RT_VK_CHECK(vkGetSwapchainImagesKHR(swapchain_context.device_combination->logical_device, swapchain_context.swapchain, &swapchain_context.image_count, vk_images.data()));

    swapchain_context.images.resize(swapchain_context.image_count);

    for (u32 i = 0; i < swapchain_context.image_count; ++i) {
        swapchain_context.images[i].image = vk_images[i];

        VkImageViewCreateInfo view_info{};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = swapchain_context.images[i].image;
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = swapchain_context.selected_surface_format.surfaceFormat.format;
        view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        RT_VK_CHECK(vkCreateImageView(swapchain_context.device_combination->logical_device, &view_info, context.allocator, &swapchain_context.images[i].view));
    }

    if (!vulkan_device_detect_depth_format(*swapchain_context.device_combination)) {
        swapchain_context.device_combination->depth_format = VK_FORMAT_UNDEFINED;
        RT_LOG_ERROR("Failed to find a supported depth format.");
    }

    vulkan_image_create(context, swapchain_context, VK_IMAGE_TYPE_2D, extent.width, extent.height, swapchain_context.device_combination->depth_format, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, true, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_TILING_OPTIMAL, swapchain_context.depth_image);

    RT_LOG_INFO("Swapchain created.");
}

void destroy(VulkanContextRef context, VulkanSwapchainContext& swapchain_context)
{
    for (u32 i = 0; i < swapchain_context.images.size(); ++i) {
        vulkan_image_destroy(context, swapchain_context, swapchain_context.images[i]);
    }
    swapchain_context.images.clear();

    vulkan_image_destroy(context, swapchain_context, swapchain_context.depth_image);

    if (swapchain_context.swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(swapchain_context.device_combination->logical_device, swapchain_context.swapchain, context.allocator);
        swapchain_context.swapchain = VK_NULL_HANDLE;
    }
}

}
