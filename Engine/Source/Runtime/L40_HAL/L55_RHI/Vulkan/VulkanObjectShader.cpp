#include "VulkanObjectShader.h"
#include "VulkanShaderUtils.h"
namespace ReiToEngine
{
b8 vulkan_object_shader_create([[maybe_unused]]VulkanContextRef ref,[[maybe_unused]] VkDevice& device,[[maybe_unused]] const char* file_path,[[maybe_unused]] VulkanObjectShader& out_shader)
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
        if(!create_shader_module(ref, file_path, stage_names[i].c_str(), stage_flags[i], i, &out_shader.shader_stages[i]))
        {
            RT_LOG_ERROR_FMT("Failed to create shader module for stage {} from file {}", stage_names[i][0], file_path);
            return false;
        }
    }
    return true;

};

}
