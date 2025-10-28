#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
namespace ReiToEngine
{
b8 vulkan_buffer_create(VulkanContextRef context_ref, u64 size, VkBufferUsageFlagBits usage, VkMemoryPropertyFlags memory_property_flags, VulkanBuffer& out_buffer)
{
    out_buffer.total_size = size;
    out_buffer.usage = usage;
    out_buffer.memory_property_flags = memory_property_flags;

    VkBufferCreateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    RT_VK_CHECK(vkCreateBuffer(context_ref.device_combination->logical_device, &buffer_info, context_ref.allocator, &out_buffer.handle));

    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(context_ref.device_combination->logical_device, out_buffer.handle, &mem_requirements);

    out_buffer.memory_index = -1;
    out_buffer.memory_index = context_ref.device_combination->find_memory_index(mem_requirements.memoryTypeBits, memory_property_flags);
    if (out_buffer.memory_index < 0)
    {
        RT_LOG_ERROR("Failed to find suitable memory type for Vulkan buffer!");
        return false;
    }

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = out_buffer.memory_index;

    RT_VK_CHECK(vkAllocateMemory(context_ref.device_combination->logical_device, &alloc_info, context_ref.allocator, &out_buffer.memory));

    return true;
}

void vulkan_buffer_destroy(VulkanContextRef context_ref, VulkanBuffer& buffer)
{
    if (buffer.memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(context_ref.device_combination->logical_device, buffer.memory, context_ref.allocator);
        buffer.memory = VK_NULL_HANDLE;
    }
    if (buffer.handle != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(context_ref.device_combination->logical_device, buffer.handle, context_ref.allocator);
        buffer.handle = VK_NULL_HANDLE;
    }
    buffer.total_size = 0;
    buffer.is_locked = false;
    buffer.memory_index = -1;
    buffer.usage = VkBufferUsageFlagBits(0);
}

b8 vulkan_buffer_resize(VulkanContextRef context_ref, u64 new_size, VkQueue queue, VkCommandPool command_pool, VulkanBuffer& buffer)
{
    VkBufferCreateInfo buffer_info;
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = new_size;
    buffer_info.usage = buffer.usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer new_buffer;
    RT_VK_CHECK(vkCreateBuffer(context_ref.device_combination->logical_device, &buffer_info, context_ref.allocator, &new_buffer));

    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(context_ref.device_combination->logical_device, new_buffer, &mem_requirements);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = buffer.memory_index;

    VkDeviceMemory new_memory;
    RT_VK_CHECK(vkAllocateMemory(context_ref.device_combination->logical_device, &alloc_info, context_ref.allocator, &new_memory));

    RT_VK_CHECK(vkBindBufferMemory(context_ref.device_combination->logical_device, new_buffer, new_memory, 0));

    vulkan_buffer_copy_to(context_ref, command_pool, VK_NULL_HANDLE, queue, buffer.handle, 0, new_buffer, 0, new_size);

    vkDeviceWaitIdle(context_ref.device_combination->logical_device);

    vulkan_buffer_destroy(context_ref, buffer);

    buffer.total_size = new_size;
    buffer.handle = new_buffer;
    buffer.memory = new_memory;
    return true;
}

void vulkan_buffer_bind(VulkanContextRef context_ref, VulkanBuffer& buffer, u64 offset)
{
    RT_VK_CHECK(vkBindBufferMemory(context_ref.device_combination->logical_device, buffer.handle, buffer.memory, offset));
}

void* vulkan_buffer_lock_memory(VulkanContextRef context_ref, VulkanBuffer& buffer, u64 offset, u64 size, u32 flags)
{
    void* data;
    RT_VK_CHECK(vkMapMemory(context_ref.device_combination->logical_device, buffer.memory, offset, size, flags, &data));
    buffer.is_locked = true;
    return data;
}
void vulkan_buffer_unlock_memory(VulkanContextRef context_ref, VulkanBuffer& buffer)
{
    vkUnmapMemory(context_ref.device_combination->logical_device, buffer.memory);
    buffer.is_locked = false;
}

void vulkan_buffer_load_data(VulkanContextRef context_ref, VulkanBuffer& buffer, u64 offset, u64 size, u32 flags, const void* data)
{
    void* data_ptr;
    RT_VK_CHECK(vkMapMemory(context_ref.device_combination->logical_device, buffer.memory, offset, size, flags, &data_ptr));
    RT_Platform_SYSCopyMemory(data_ptr, data, size);
    vkUnmapMemory(context_ref.device_combination->logical_device, buffer.memory);
}

void vulkan_buffer_copy_to(VulkanContextRef context_ref, VkCommandPool command_pool,[[maybe_unused]]VkFence fence, VkQueue queue,  VkBuffer src_buffer, u64 src_offset, VkBuffer dst_buffer,u64 dest_offset, u64 size)
{
    vkQueueWaitIdle(queue);

    VulkanCommandBuffer command_buffer;
    vulkan_command_buffer_allocate_and_begin_single_use(context_ref.device_combination->logical_device, command_pool, command_buffer);

    VkBufferCopy copy_region{};
    copy_region.srcOffset = src_offset;
    copy_region.dstOffset = dest_offset;
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer.handle, src_buffer, dst_buffer, 1, &copy_region);

    vulkan_command_buffer_end_single_use(context_ref.device_combination->logical_device, command_pool, command_buffer, queue);
}
}
