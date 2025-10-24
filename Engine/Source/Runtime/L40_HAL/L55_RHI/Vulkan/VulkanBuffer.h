#ifndef RHI_VULKAN_BUFFER_H
#define RHI_VULKAN_BUFFER_H
#include "VulkanDefines.h"
namespace ReiToEngine
{
b8 vulkan_buffer_create(VulkanContextRef context_ref, u64 size, VkBufferUsageFlagBits usage, VkMemoryPropertyFlags memory_property_flags, VulkanBuffer& out_buffer);

void vulkan_buffer_destroy(VulkanContextRef context_ref, VulkanBuffer& buffer);

b8 vulkan_buffer_resize(VulkanContextRef context_ref, u64 new_size, VkQueue queue, VkCommandPool command_pool, VulkanBuffer& buffer);

void vulkan_buffer_bind(VulkanContextRef context_ref, VulkanBuffer& buffer, u64 offset);

void* vulkan_buffer_lock_memory(VulkanContextRef context_ref, VulkanBuffer& buffer, u64 offset, u64 size, u32 flags);
void vulkan_buffer_unlock_memory(VulkanContextRef context_ref, VulkanBuffer& buffer);
void vulkan_buffer_load_data(VulkanContextRef context_ref, VulkanBuffer& buffer, u64 offset, u64 size, u32 flags, const void* data);

void vulkan_buffer_copy_to(VulkanContextRef context_ref, VkCommandPool command_pool,VkFence fence, VkQueue queue,  VulkanBuffer& src_buffer, u64 src_offset, VulkanBuffer& dst_buffer,u64 dest_offset, u64 size);
}
#endif
