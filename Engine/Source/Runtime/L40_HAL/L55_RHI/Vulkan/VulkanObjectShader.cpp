#include "VulkanObjectShader.h"
#include "VulkanShaderUtils.h"
#include "VulkanPipeline.h"
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

    List<VkPipelineShaderStageCreateInfo> shader_stages;

    shader_stages.resize(valid_stage_count);

    RT_LOG_DEBUG_FMT("Creating graphics pipeline for shader from file {}, valid shader stage size: {}", file_path, valid_stage_count);

    for (u8 i = 0; i < valid_stage_count; ++i)
    {
        shader_stages[i].sType = out_shader.shader_stages[i].stage_create_info.sType;
        shader_stages[i] = out_shader.shader_stages[i].stage_create_info;
    }

    if(vulkan_graphics_pipeline_create(ref, attribute_count, attributes, 0, nullptr, shader_stages.size(), shader_stages.data(), &viewport, &scissor, false, out_shader.pipeline))
    {
        RT_LOG_DEBUG_FMT("Succeeded to create graphics pipeline for shader from file {}", file_path);
        return true;
    };
    return false;
};

void vulkan_object_shader_destroy(VkDevice& device, VulkanShaderSet& shader)
{
    vulkan_graphics_pipeline_destroy(device, shader.pipeline);
    for (u8 i = 0; i < static_cast<u8>(ShaderStageType::MAX); ++i)
    {
        if (shader.shader_stages[i].handle != VK_NULL_HANDLE) {
            vkDestroyShaderModule(device, shader.shader_stages[i].handle, nullptr);
            shader.shader_stages[i].handle = VK_NULL_HANDLE;
        }
    }
}

}
