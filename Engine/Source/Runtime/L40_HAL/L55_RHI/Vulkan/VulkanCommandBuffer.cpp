#include "VulkanCommandBuffer.h"

namespace ReiToEngine
{
void vulkan_command_buffer_allocate(VkDevice& device, VkCommandPool commandPool, b8 is_primary, VulkanCommandBuffer& out_command_buffer)
{
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = commandPool;
    alloc_info.level = is_primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    alloc_info.commandBufferCount = 1;
    alloc_info.pNext = nullptr;

    out_command_buffer.state = VulkanCommandBufferState::NOT_ALLOCATED;

    RT_VK_CHECK(vkAllocateCommandBuffers(device, &alloc_info, &out_command_buffer.command_buffer));
    out_command_buffer.state = VulkanCommandBufferState::READY;
}

void vulkan_command_buffer_free(VkDevice& device, VkCommandPool commandPool, VulkanCommandBuffer& out_command_buffer)
{
    vkFreeCommandBuffers(device, commandPool, 1, &out_command_buffer.command_buffer);
    out_command_buffer.command_buffer = VK_NULL_HANDLE;
    out_command_buffer.state = VulkanCommandBufferState::NOT_ALLOCATED;
}

void vulkan_command_buffer_begin(VulkanCommandBuffer& command_buffer, b8 is_single_use, b8 is_render_pass_continue, b8 is_simultaneous_use)
{
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0;
    if (is_single_use) {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    if (is_render_pass_continue) {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    if (is_simultaneous_use) {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }

    begin_info.pNext = nullptr;

    RT_VK_CHECK(vkBeginCommandBuffer(command_buffer.command_buffer, &begin_info));
    command_buffer.state = VulkanCommandBufferState::RECORDING;
}

void vulkan_command_buffer_end(VulkanCommandBuffer& command_buffer)
{
    RT_VK_CHECK(vkEndCommandBuffer(command_buffer.command_buffer));
    command_buffer.state = VulkanCommandBufferState::RECORDED;
}

void vulkan_command_buffer_update_submmitted(VulkanCommandBuffer& command_buffer)
{
    command_buffer.state = VulkanCommandBufferState::SUBMITTED;
}

void vulkan_command_buffer_reset(VulkanCommandBuffer& command_buffer)
{
    command_buffer.state = VulkanCommandBufferState::READY;
}

void vulkan_command_buffer_allocate_and_begin_single_use(VkDevice& device, VkCommandPool commandPool, VulkanCommandBuffer& out_command_buffer)
{
    vulkan_command_buffer_allocate(device, commandPool, true, out_command_buffer);
    vulkan_command_buffer_begin(out_command_buffer, true, false, false);
}

void vulkan_command_buffer_end_single_use(VkDevice& device, VkCommandPool commandPool, VulkanCommandBuffer& out_command_buffer, VkQueue queue)
{
    vulkan_command_buffer_end(out_command_buffer);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &out_command_buffer.command_buffer;
    submit_info.pNext = nullptr;

    RT_VK_CHECK(vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE));
    RT_VK_CHECK(vkQueueWaitIdle(queue));

    vulkan_command_buffer_free(device, commandPool, out_command_buffer);
}
}
