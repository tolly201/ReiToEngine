#include "VulkanObjectShader.h"
#include "VulkanShaderUtils.h"
#include "VulkanPipeline.h"
#include "VulkanBuffer.h"
namespace ReiToEngine
{
b8 vulkan_object_shader_create(VulkanContextRef ref, const char* file_path,VulkanShaderSet& out_shader)
{
    String stage_names[static_cast<u8>(ShaderStageType::MAX)] =
    {
        "vert",
        "frag",
        "comp",
    };

    VkShaderStageFlagBits stage_flags[static_cast<u8>(ShaderStageType::MAX)] =
    {
        VK_SHADER_STAGE_VERTEX_BIT,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        VK_SHADER_STAGE_COMPUTE_BIT,
    };

    for (u8 i = 0; i < static_cast<u8>(ShaderStageType::MAX); ++i)
    {
        if(!create_shader_module(ref, file_path, stage_names[i].c_str(), stage_flags[i], out_shader.shader_stages[i]))
        {
            RT_LOG_WARN_FMT_PLATFORM("Failed to create shader module for stage {} from file {}", stage_names[i][0], file_path);
            // return false;
        }
    }

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = ref.swapchain_context->width;
    viewport.height = ref.swapchain_context->height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent.width = ref.swapchain_context->width;
    scissor.extent.height = ref.swapchain_context->height;

    u32 offset = 0;
    const i32 attribute_count = 1;
    VkVertexInputAttributeDescription attributes[attribute_count]{};

    VkFormat formats[attribute_count] =
    {
        VK_FORMAT_R32G32B32_SFLOAT, // position
    };

    u64 sizes[attribute_count] =
    {
        sizeof(Vec3f), // position
    };

    for (u32 i = 0; i < attribute_count; ++i)
    {
        attributes[i].binding = 0;
        attributes[i].location = i;
        attributes[i].format = formats[i];
        attributes[i].offset = offset;
        offset += sizes[i];
    }

    u32 valid_stage_count = 0;
    for (u8 i = 0; i < static_cast<u8>(ShaderStageType::MAX); ++i)
    {
        if (out_shader.shader_stages[i].is_valid) {
            valid_stage_count++;
        }
    }

    const i32 descriptor_set_layout_count = 1;
    VkDescriptorSetLayout layouts[1] = {
        ref.device_combination->set0_global_layout
    };

    List<VkPipelineShaderStageCreateInfo> shader_stages;

    shader_stages.resize(valid_stage_count);

    RT_LOG_DEBUG_FMT("Creating graphics pipeline for shader from file {}, valid shader stage size: {}", file_path, valid_stage_count);

    for (u8 i = 0; i < valid_stage_count; ++i)
    {
        shader_stages[i].sType = out_shader.shader_stages[i].stage_create_info.sType;
        shader_stages[i] = out_shader.shader_stages[i].stage_create_info;
    }

    if(!vulkan_graphics_pipeline_create(ref, attribute_count, attributes, descriptor_set_layout_count, layouts, shader_stages.size(), shader_stages.data(), &viewport, &scissor, false, out_shader.pipeline))
    {
        RT_LOG_DEBUG_FMT("Failed to create graphics pipeline for shader from file {}", file_path);
        return false;
    };

    if (!vulkan_buffer_create(ref, sizeof(global_uniform_object), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, out_shader.global_uniform_buffer))
    {
        RT_LOG_WARN_FMT_PLATFORM("Failed to create global uniform buffer for shader from file {}", file_path);
        return false;
    }
    vulkan_buffer_bind(ref, out_shader.global_uniform_buffer, 0);

    VkDescriptorSetLayout _layouts[3] = {
        ref.device_combination->set0_global_layout,
        ref.device_combination->set1_material_layout,
        ref.device_combination->set2_object_layout
    };

    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = ref.swapchain_context->swapchain_per_frame_descriptor_pool;
    alloc_info.descriptorSetCount = 3;
    alloc_info.pSetLayouts = _layouts;

    out_shader.description_set.resize(3);

    RT_VK_CHECK(vkAllocateDescriptorSets(ref.device_combination->logical_device, &alloc_info, out_shader.description_set.data()));

    return true;
};

void vulkan_object_shader_destroy(VulkanContextRef ref, VulkanShaderSet& shader)
{
    vulkan_buffer_destroy(ref, shader.global_uniform_buffer);

    vulkan_graphics_pipeline_destroy(ref.device_combination->logical_device, shader.pipeline);
    for (u8 i = 0; i < static_cast<u8>(ShaderStageType::MAX); ++i)
    {
        if (shader.shader_stages[i].handle != VK_NULL_HANDLE) {
            vkDestroyShaderModule(ref.device_combination->logical_device, shader.shader_stages[i].handle, nullptr);
            shader.shader_stages[i].handle = VK_NULL_HANDLE;
        }
    }
}

void vulkan_object_shader_use([[maybe_unused]]VulkanContextRef ref, VkCommandBuffer& cmd_buffer, VulkanShaderSet& shader)
{
    vulkan_graphics_pipeline_bind(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader.pipeline);
}

void vulkan_object_shader_update_global_state(VulkanContextRef ref, VulkanShaderSet& shader)
{
    u32 image_index = ref.swapchain_context->current_image_index;
    VkCommandBuffer command_buffer = ref.device_combination->command_buffers[VulkanQueueFamilyIndicesType::GRAPHICS][image_index].handle;
    VkDescriptorSet descriptor_set = shader.description_set[image_index];

    vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader.pipeline.layout, 0, 1, &descriptor_set, 0, nullptr);

    u32 range = sizeof(global_uniform_object);
    u32 offset = 0;

    vulkan_buffer_load_data(ref, shader.global_uniform_buffer, offset, range, 0, &shader.global_ubo);

    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer = shader.global_uniform_buffer.handle;
    buffer_info.offset = offset;
    buffer_info.range = range;

    VkWriteDescriptorSet descriptor_write{};
    descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet = shader.description_set[image_index];
    descriptor_write.dstBinding = 0;
    descriptor_write.dstArrayElement = 0;
    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_write.descriptorCount = 1;
    descriptor_write.pBufferInfo = &buffer_info;

    vkUpdateDescriptorSets(ref.device_combination->logical_device, 1, &descriptor_write, 0, nullptr);
}
}
