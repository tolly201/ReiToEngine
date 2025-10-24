#ifndef RHI_VULKAN_SHADER_UTILITIES_H
#define RHI_VULKAN_SHADER_UTILITIES_H
#include "vulkan/vulkan.h"
#include "../RendererTypes.h"
#include "VulkanRendererPlatform.h"
#include "VulkanDefines.h"

namespace ReiToEngine
{
b8 create_shader_module(VulkanContextRef context, const char* name, const char* type_str, VkShaderStageFlagBits stage, VulkanShaderStage& out_shader_stage);
}
#endif
