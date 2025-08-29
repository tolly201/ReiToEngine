#include "L20_Platform/Include.h"
#ifdef RT_SYSTEM_WINDOWS
#include "VulkanRendererPlatform.h"
#include "vulkan/vulkan.h"
namespace ReiToEngine
{
void PlatformGetVulkanExtensions(ReiToEngine::List<const char*>& out_extensions)
{
}
};

#endif
