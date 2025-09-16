#ifndef RHI_VULKAN_COMMAND_BUFFER_H
#define RHI_VULKAN_COMMAND_BUFFER_H
#include "vulkan/vulkan.h"
#include "L20_Platform/Include.h"
#include "../RendererTypes.h"
#include "VulkanRendererPlatform.h"

namespace ReiToEngine
{
void vulkan_command_buffer_allocate(VkDevice& device, VkCommandPool commandPool, b8 is_primary, VulkanCommandBuffer& out_command_buffer);

void vulkan_command_buffer_free(VkDevice& device, VkCommandPool commandPool, VulkanCommandBuffer& out_command_buffer);

void vulkan_command_buffer_begin(VulkanCommandBuffer& command_buffer, b8 is_single_use, b8 is_render_pass_continue, b8 is_simultaneous_use);
void vulkan_command_buffer_end(VulkanCommandBuffer& command_buffer);

void vulkan_command_buffer_update_submmitted(VulkanCommandBuffer& command_buffer);

void vulkan_command_buffer_reset(VulkanCommandBuffer& command_buffer);

void vulkan_command_buffer_allocate_and_begin_single_use(VkDevice& device, VkCommandPool commandPool, VulkanCommandBuffer& out_command_buffer);
void vulkan_command_buffer_end_single_use(VkDevice& device, VkCommandPool commandPool, VulkanCommandBuffer& out_command_buffer, VkQueue queue);
}


#endif
