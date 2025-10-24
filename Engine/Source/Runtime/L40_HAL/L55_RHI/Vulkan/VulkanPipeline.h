#ifndef RHI_VULKAN_PIPELINE_H
#define RHI_VULKAN_PIPELINE_H
#include "VulkanDefines.h"
namespace ReiToEngine
{
b8 vulkan_graphics_pipeline_create(VulkanContextRef ref, u32 attribute_count, VkVertexInputAttributeDescription* attributes, u32 descriptor_set_layout_count, VkDescriptorSetLayout* descriptor_set_layouts, u8 stage_count, VkPipelineShaderStageCreateInfo* stages, VkViewport* viewport, VkRect2D* scissor, b8 is_wireframe, VulkanPipeline& out_pipeline);
void vulkan_graphics_pipeline_destroy(VkDevice& device, VulkanPipeline& pipeline);

void vulkan_graphics_pipeline_bind(VkCommandBuffer& cmd_buffer, VkPipelineBindPoint bind_point, VulkanPipeline& pipeline);
}
#endif
