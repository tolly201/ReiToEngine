#include "VulkanFence.h"
namespace ReiToEngine
{
void vulkan_fence_create(VulkanContextRef ref, VkDevice& device, b8 is_signaled, VulkanFence& out_fence)
{
    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    out_fence.signaled = is_signaled;
    fence_info.flags = is_signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
    fence_info.pNext = nullptr;

    RT_VK_CHECK(vkCreateFence(device, &fence_info, ref.allocator, &out_fence.handle));
}

void vulkan_fence_destroy(VulkanContextRef ref, VkDevice& device, VulkanFence& fence)
{
    if (fence.handle != VK_NULL_HANDLE) {
        vkDestroyFence(device, fence.handle, ref.allocator);
        fence.handle = VK_NULL_HANDLE;
        fence.signaled = false;
    }
}

b8 vulkan_fence_wait(VkDevice& device, VulkanFence& fence, u64 timeout)
{
    if (fence.signaled) {
        return true;
    } else{
        VkResult result = vkWaitForFences(device, 1, &fence.handle, VK_TRUE, timeout);
        switch (result)
        {
        case VK_SUCCESS:
            fence.signaled = true;
            return true;
        case VK_TIMEOUT:
            RT_LOG_WARN_PLATFORM("Vulkan Fence wait timed out.");
            return false;
        case VK_ERROR_DEVICE_LOST:
            RT_LOG_ERROR_PLATFORM("Vulkan Fence wait failed: VK_ERROR_DEVICE_LOST");
            return false;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            RT_LOG_ERROR_PLATFORM("Vulkan Fence wait failed: VK_ERROR_OUT_OF_HOST_MEMORY");
            return false;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            RT_LOG_ERROR_PLATFORM("Vulkan Fence wait failed: VK_ERROR_OUT_OF_DEVICE_MEMORY");
            return false;
        default:
            RT_LOG_ERROR_PLATFORM("Vulkan Fence wait failed: Unknown error");
            return false;
        }
    }
    return false;
}
b8 vulkan_fence_reset(VkDevice& device, VulkanFence& fence)
{
    if (!fence.signaled) {
        return true;
    } else {
        VkResult result = vkResetFences(device, 1, &fence.handle);
        switch (result)
        {
        case VK_SUCCESS:
            fence.signaled = false;
            return true;
        case VK_ERROR_DEVICE_LOST:
            RT_LOG_ERROR_PLATFORM("Vulkan Fence reset failed: VK_ERROR_DEVICE_LOST");
            return false;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            RT_LOG_ERROR_PLATFORM("Vulkan Fence reset failed: VK_ERROR_OUT_OF_HOST_MEMORY");
            return false;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            RT_LOG_ERROR_PLATFORM("Vulkan Fence reset failed: VK_ERROR_OUT_OF_DEVICE_MEMORY");
            return false;
        default:
            RT_LOG_ERROR_PLATFORM("Vulkan Fence reset failed: Unknown error");
            return false;
        }
    }
    return false;
}
}
