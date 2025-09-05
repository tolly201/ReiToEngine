#ifndef RHI_VULKAN_EXTENSIONS_H
#define RHI_VULKAN_EXTENSIONS_H
#include "vulkan/vulkan.h"
#include "L20_Platform/Include.h"
#include "../RendererTypes.h"
namespace ReiToEngine
{

void get_vulkan_extensions(u32& out_extension_count, ReiToEngine::List<const char*>& out_extensions);
b8 get_vulkan_validation_layers(u32& out_layer_count, ReiToEngine::List<const char*>& out_layers);

b8 create_debugger(VkInstance& instance, VkAllocationCallbacks*& allocator, VkDebugUtilsMessengerEXT& debug_messenger);

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageTypes,const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData, void* pUserData);

}

#endif
