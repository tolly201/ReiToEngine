#ifndef RHI_VULKAN_OBJECT_SHADER_H
#define RHI_VULKAN_OBJECT_SHADER_H
#include "VulkanDefines.h"
namespace ReiToEngine
{
b8 vulkan_object_shader_create(VulkanContextRef ref, VkDevice& device, const char* file_path, VulkanObjectShader& out_shader);
void vulkan_object_shader_destroy(VkDevice& device, VulkanObjectShader& shader);
void vulkan_object_shader_use(VkCommandBuffer& cmd_buffer, VulkanObjectShader& shader);
}
#endif
