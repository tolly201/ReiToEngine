#ifndef RHI_VULKAN_IMAGE_H
#define RHI_VULKAN_IMAGE_H
#include "vulkan/vulkan.h"
#include "../RendererTypes.h"
#include "VulkanRendererPlatform.h"
namespace ReiToEngine
{
void vulkan_image_create(VulkanContextRef context_ref, VulkanSwapchainContext& swapchain_context, VkImageType type, u32 width, u32 height, VkFormat format, VkImageUsageFlags usage, VkMemoryPropertyFlags memort_properties, b32 create_view, VkImageAspectFlags aspect_flags, VkImageTiling tiling, VulkanImage& out_image);

void vulkan_image_view_create(VulkanContextRef context_ref, VulkanSwapchainContext& swapchain_context, VkFormat format, VulkanImage& image, VkImageAspectFlags aspect_flags);

void vulkan_image_destroy(VulkanContextRef context_ref, VulkanSwapchainContext& swapchain_context, VulkanImage& image);
}
#endif
