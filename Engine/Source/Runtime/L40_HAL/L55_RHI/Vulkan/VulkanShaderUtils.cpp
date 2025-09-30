
#include "VulkanShaderUtils.h"
#include "L40_HAL/L40_File/CFileManager.h"
namespace ReiToEngine
{
b8 create_shader_module(VulkanContextRef context, const char* name, const char* type_str, [[maybe_unused]]VkShaderStageFlagBits stage, u32 stage_index, VulkanShaderStage* out_shader_stages){
    String filename = String::Format("Assets/Shaders/%s.%s.spv", name, type_str);
    RT_LOG_DEBUG_FMT("Loading shader file: {}", filename.c_str());

    out_shader_stages[stage_index].module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    RTCFileManager::Instance().FileExists(filename.c_str());
    RTFArchive* file = RTCFileManager::Instance().CreateDebugFileWriter(filename.c_str());
    if (!file->IsValid()) {
        RT_LOG_ERROR_FMT("Failed to open shader file: {}", filename.c_str());
        return false;
    }

    RT_LOG_DEBUG_FMT("file size: {}", file->GetFileSize());

    out_shader_stages[stage_index].module_create_info.pNext = nullptr;
    out_shader_stages[stage_index].module_create_info.codeSize = file->GetFileSize();
    out_shader_stages[stage_index].module_create_info.pCode = static_cast<u32*>(file->GetBuffer());

    RT_VK_CHECK(vkCreateShaderModule(context.device_combination->logical_device, &out_shader_stages[stage_index].module_create_info, context.allocator, &out_shader_stages[stage_index].handle));

    file->Close();
    delete file;

    out_shader_stages[stage_index].stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    out_shader_stages[stage_index].stage_create_info.pNext = nullptr;
    out_shader_stages[stage_index].stage_create_info.flags = 0;
    out_shader_stages[stage_index].stage_create_info.stage = stage;
    out_shader_stages[stage_index].stage_create_info.module = out_shader_stages[stage_index].handle;
    out_shader_stages[stage_index].stage_create_info.pName = "main";

    return true;
}
}
