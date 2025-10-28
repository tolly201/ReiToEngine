#ifndef RHI_VULKAN_OBJECT_SHADER_H
#define RHI_VULKAN_OBJECT_SHADER_H
#include "VulkanDefines.h"
namespace ReiToEngine
{
b8 vulkan_object_shader_create(VulkanContextRef ref, const char* file_path, VulkanShaderSet& out_shader);
void vulkan_object_shader_destroy(VkDevice& device, VulkanShaderSet& shader);
void vulkan_object_shader_use([[maybe_unused]]VulkanContextRef ref, VkCommandBuffer& cmd_buffer, VulkanShaderSet& shader);
}
#endif
