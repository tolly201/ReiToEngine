#ifndef RHI_VULKAN_FRAME_BUFFER_H
#define RHI_VULKAN_FRAME_BUFFER_H
#include "VulkanDefines.h"
namespace ReiToEngine
{
void vulkan_frame_buffer_create(VulkanContextRef context, VulkanSwapchainContext& swapchain_context,
VulkanRenderPass& render_pass, u32 width, u32 height,
u32 attachment_count, VkImageView* p_attachments,
VulkanFrameBuffer& out_framebuffer);

void vulkan_frame_buffer_destroy(VulkanContextRef context, VulkanSwapchainContext& , VulkanFrameBuffer& framebuffer);

void vulkan_command_buffer_begin(VulkanCommandBuffer& command_buffer, b8 is_single_use, b8 is_render_pass_continue, b8 is_simultaneous_use);
void vulkan_command_buffer_end(VulkanCommandBuffer& command_buffer);

void vulkan_command_buffer_update_submmitted(VulkanCommandBuffer& command_buffer);

void vulkan_command_buffer_reset(VulkanCommandBuffer& command_buffer);

void vulkan_command_buffer_allocate_and_begin_single_use(VkDevice& device, VkCommandPool commandPool, VulkanCommandBuffer& out_command_buffer);
void vulkan_command_buffer_end_single_use(VkDevice& device, VkCommandPool commandPool, VulkanCommandBuffer& out_command_buffer, VkQueue queue);
}


#endif
