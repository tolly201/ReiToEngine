#ifndef RHI_VULKAN_RENDERPASS_H
#define RHI_VULKAN_RENDERPASS_H
#include "vulkan/vulkan.h"
#include "../RendererTypes.h"
#include "VulkanRendererPlatform.h"
#include "VulkanDefines.h"

namespace ReiToEngine
{
void vulkan_renderpass_create(VulkanContextRef context, VulkanSwapchainContext& swapchain_context, VulkanRenderPass& out_render_pass);

void vulkan_renderpass_destroy(VulkanContextRef context, VulkanSwapchainContext& swapchain_context, VulkanRenderPass& out_render_pass);

void vulkan_renderpass_begin(VulkanContextRef context, VulkanSwapchainContext& swapchain_context,VulkanRenderPass& render_pass, VulkanCommandBuffer& command_buffer, VkFramebuffer framebuffer);

void vulkan_renderpass_end(VulkanContextRef context, VulkanSwapchainContext& swapchain_context,VulkanRenderPass& render_pass, VulkanCommandBuffer& command_buffer, VkFramebuffer framebuffer);
}
#endif
