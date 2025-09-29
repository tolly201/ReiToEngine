#include "VulkanExtensions.h"
#include "VulkanRendererPlatform.h"
#include "VulkanDefines.h"
namespace ReiToEngine
{
void get_vulkan_extensions(u32& out_extension_count, ReiToEngine::List<const char*>& out_extensions)
{
    out_extension_count = 0;
    out_extensions.clear();
#if RT_OHI_DEBUG == 1
    out_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    out_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif
    PlatformGetVulkanExtensions(out_extensions);

    out_extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
    out_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    out_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    out_extension_count = out_extensions.size();
    RT_LOG_INFO("Vulkan Extension: ");
    for(auto& ext : out_extensions) {
        RT_LOG_INFO(ext);
    }
}

b8 get_vulkan_validation_layers(u32& out_layer_count, ReiToEngine::List<const char*>& out_layers)
{
    out_layer_count = 0;
    out_layers.clear();
#if RT_OHI_DEBUG == 1
    RT_LOG_INFO("Vulkan Validation Layers enabled.");
    out_layers.push_back("VK_LAYER_KHRONOS_validation");

    u32 available_layer_count = 0;
    vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr);
    ReiToEngine::List<VkLayerProperties> available_layers;
    available_layers.resize(available_layer_count);
    vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers.data());

    for (u32 i = 0; i < out_layers.size(); ++i) {
        RT_LOG_INFO("Requested Vulkan Validation Layer: ", out_layers[i]);
        b8 layer_found = false;

        for (u32 j = 0; j < available_layer_count; ++j) {
            if (RT_StrCmp(out_layers[i], available_layers[j].layerName)) {
                layer_found = true;
                break;
            }
        }
        if (!layer_found) {
            RT_LOG_FATAL("Vulkan Validation Layer not found: ", out_layers[i]);
            out_layers.clear();
            out_layer_count = 0;
            return false;
        }
    }

    RT_LOG_INFO("All requested Vulkan Validation Layers are available.");
    out_layer_count = out_layers.size();
    return true;
#endif
}

b8 create_debugger(VkInstance& instance, VkAllocationCallbacks*& allocator, VkDebugUtilsMessengerEXT& debug_messenger)
{
#if RT_OHI_DEBUG == 1
    u32 msg_severity =VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT    |
                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_create_info.messageSeverity = msg_severity;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;
    debug_create_info.pUserData = nullptr;

    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    RT_ASSERT_MESSAGE(func != nullptr, "Failed to get vkCreateDebugUtilsMessengerEXT function.");
    RT_VK_CHECK(func(instance, &debug_create_info, allocator, &debug_messenger));
    RT_LOG_DEBUG("Vulkan Debug Messenger created.");
    return true;
#endif
    debug_messenger = VK_NULL_HANDLE;
    return false;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
    [[maybe_unused]]VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData,
    [[maybe_unused]]void*                                        pUserData)
{
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        RT_LOG_ERROR("Vulkan Validation Layer: ", pCallbackData->pMessage);
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        RT_LOG_WARN("Vulkan Validation Layer: ", pCallbackData->pMessage);
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        RT_LOG_INFO("Vulkan Validation Layer: ", pCallbackData->pMessage);
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        RT_LOG_TRACE("Vulkan Validation Layer: ", pCallbackData->pMessage);
    }
    return VK_FALSE;
}
}
