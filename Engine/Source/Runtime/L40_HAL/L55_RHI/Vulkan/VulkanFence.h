#ifndef RHI_VULKAN_FENCE_H
#define RHI_VULKAN_FENCE_H
#include "VulkanDefines.h"
namespace ReiToEngine
{
void vulkan_fence_create(VulkanContextRef ref, VkDevice& device, b8 is_signaled, VulkanFence& out_fence);
void vulkan_fence_destroy(VulkanContextRef ref, VkDevice& device, VulkanFence& fence);
b8 vulkan_fence_wait(VkDevice& device, VulkanFence& fence, u64 timeout);
b8 vulkan_fence_reset(VkDevice& device, VulkanFence& fence);
}
#endif
