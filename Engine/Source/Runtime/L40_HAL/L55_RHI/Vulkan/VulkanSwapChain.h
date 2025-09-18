#ifndef RHI_VULKAN_SWAPCHAIN_H
#define RHI_VULKAN_SWAPCHAIN_H
#include "vulkan/vulkan.h"
#include "../RendererTypes.h"
#include "VulkanRendererPlatform.h"
#include "VulkanDefines.h"

namespace ReiToEngine
{
void vulkan_swapchain_create(VulkanContextRef context, VulkanSwapchainContext& swapchain_context);

b8 vulkan_swapchain_recreate(VulkanContextRef context, VulkanSwapchainContext& swapchain_context);

void vulkan_swapchain_destroy(VulkanContextRef context, VulkanSwapchainContext& swapchain_context);

b8 vulkan_swapchain_acquire_next_image_index(VulkanContextRef context ,VulkanSwapchainContext& swapchain_context, u32 timeout_us, VkSemaphore& semaphore, VkFence* fence, u32& image_index);

b8 vulkan_swapchain_present(VulkanContextRef context ,VulkanSwapchainContext& swapchain_context, VkQueue graphics_queue, VkQueue present_queue, VkSemaphore complete_semaphore, u32 present_image_index);
}
#endif
