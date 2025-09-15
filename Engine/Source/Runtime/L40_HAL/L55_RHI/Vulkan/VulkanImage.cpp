#include "VulkanImage.h"
namespace ReiToEngine
{
void vulkan_image_create(VulkanContextRef context, VulkanSwapchainContext& swapchain_context, VkImageType type, u32 width, u32 height, VkFormat format, VkImageUsageFlags usage, VkMemoryPropertyFlags memort_properties, b32 create_view, VkImageAspectFlags aspect_flags, VkImageTiling tiling, VulkanImage& out_image)
{
    out_image.width = width;
    out_image.height = height;
    out_image.format = format;

    VkImageCreateInfo image_info = {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = type;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 4;
    image_info.arrayLayers = 1;
    image_info.format = format;
    image_info.tiling = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    RT_VK_CHECK(vkCreateImage(swapchain_context.device_combination->logical_device, &image_info, context.allocator, &out_image.image));

    VkMemoryRequirements2 mem_requirements;
    VkImageMemoryRequirementsInfo2 mem_info = {};
    mem_info.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2;
    mem_info.image = out_image.image;
    mem_info.pNext = nullptr;
    mem_requirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
    mem_requirements.pNext = nullptr;
    vkGetImageMemoryRequirements2(swapchain_context.device_combination->logical_device, &mem_info, &mem_requirements);

    u32 type_bits = mem_requirements.memoryRequirements.memoryTypeBits;

    i32 memory_type = swapchain_context.device_combination->find_memory_index(type_bits, memort_properties);

    if (memory_type == -1) {
        RT_LOG_ERROR("Failed to find suitable memory type for image.");
        return;
    }

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.memoryRequirements.size;
    alloc_info.memoryTypeIndex = memory_type;

    RT_VK_CHECK(vkAllocateMemory(swapchain_context.device_combination->logical_device, &alloc_info, context.allocator, &out_image.memory));

    RT_VK_CHECK(vkBindImageMemory(swapchain_context.device_combination->logical_device, out_image.image, out_image.memory, 0));

    if (create_view) {
        out_image.view = VK_NULL_HANDLE;
        vulkan_image_view_create(context, swapchain_context, format, out_image, aspect_flags);
    }
}

void vulkan_image_view_create(VulkanContextRef context_ref, VulkanSwapchainContext& swapchain_context, VkFormat format, VulkanImage& image, VkImageAspectFlags aspect_flags)
{
    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image.image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    view_info.subresourceRange.aspectMask = aspect_flags;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    RT_VK_CHECK(vkCreateImageView(swapchain_context.device_combination->logical_device, &view_info, context_ref.allocator, &image.view));
}

void vulkan_image_destroy(VulkanContextRef context_ref, VulkanSwapchainContext& swapchain_context, VulkanImage& image)
{
    if (image.view != VK_NULL_HANDLE) {
        vkDestroyImageView(swapchain_context.device_combination->logical_device, image.view, context_ref.allocator);
        image.view = VK_NULL_HANDLE;
    }

    if (image.memory != VK_NULL_HANDLE) {
        vkFreeMemory(swapchain_context.device_combination->logical_device, image.memory, context_ref.allocator);
        image.memory = VK_NULL_HANDLE;

        if (image.image != VK_NULL_HANDLE) {
            vkDestroyImage(swapchain_context.device_combination->logical_device, image.image, context_ref.allocator);
            image.image = VK_NULL_HANDLE;
        }

    }

    // if (image.image != VK_NULL_HANDLE) {
    //     vkDestroyImage(swapchain_context.device_combination->logical_device, image.image, context_ref.allocator);
    //     image.image = VK_NULL_HANDLE;
    // }
}

}
