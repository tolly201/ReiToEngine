#ifndef RHI_VULKAN_RENDERER_PLATFORM_H
#define RHI_VULKAN_RENDERER_PLATFORM_H
#include "vulkan/vulkan.h"
#include "L20_Platform/Include.h"
namespace ReiToEngine
{
void PlatformGetVulkanExtensions(ReiToEngine::List<const char*>& out_extensions);
};

#endif
