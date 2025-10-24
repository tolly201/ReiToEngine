
#include "VulkanShaderUtils.h"
#include "L20_Platform/L26_File/CFileManager.h"
namespace ReiToEngine
{
b8 create_shader_module(VulkanContextRef context, const char* name, const char* type_str, [[maybe_unused]]VkShaderStageFlagBits stage, VulkanShaderStage& out_shader_stage){
    String filename = String::Format("Assets/Shaders/%s.%s.spv", name, type_str);
    RT_LOG_DEBUG_FMT_PLATFORM("Loading shader file: {}", filename.c_str());

    out_shader_stage.is_valid = false;

    RTCFileManager::Instance().FileExists(filename.c_str());
    RTFArchive* file = RTCFileManager::Instance().CreateDebugFileWriter(filename.c_str());
    if (!file->IsValid()) {
        RT_LOG_WARN_FMT_PLATFORM("Failed to open shader file: {}", filename.c_str());
        return false;
    }

    out_shader_stage.module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    RT_LOG_DEBUG_FMT_PLATFORM("file size: {}", file->GetFileSize());

    out_shader_stage.module_create_info.pNext = nullptr;
    out_shader_stage.module_create_info.codeSize = file->GetFileSize();
    out_shader_stage.module_create_info.pCode = static_cast<u32*>(file->GetBuffer());

    RT_VK_CHECK(vkCreateShaderModule(context.device_combination->logical_device, &out_shader_stage.module_create_info, context.allocator, &out_shader_stage.handle));

    file->Close();
    delete file;

    out_shader_stage.stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    out_shader_stage.stage_create_info.pNext = nullptr;
    out_shader_stage.stage_create_info.flags = 0;
    out_shader_stage.stage_create_info.stage = stage;
    out_shader_stage.stage_create_info.module = out_shader_stage.handle;
    out_shader_stage.stage_create_info.pName = "main";
    out_shader_stage.is_valid = true;
    return true;
}
}
