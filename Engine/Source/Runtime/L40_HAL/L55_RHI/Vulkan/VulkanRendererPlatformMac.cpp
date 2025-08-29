#include "L20_Platform/Include.h"
#ifdef RT_SYSTEM_APPLE
#include "VulkanRendererPlatform.h"
#include "vulkan/vulkan.h"
namespace ReiToEngine
{
void PlatformGetVulkanExtensions(ReiToEngine::List<const char*>& out_extensions)
{
    out_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
}
};

#endif
