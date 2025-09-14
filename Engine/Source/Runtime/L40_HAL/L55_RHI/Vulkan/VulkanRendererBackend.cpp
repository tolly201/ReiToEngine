#include "VulkanRendererBackend.h"
#include "L20_Platform/Include.h"
#include "VulkanExtensions.h"
#include "VulkanDevices.h"
#include "VulkanSwapChain.h"
namespace ReiToEngine
{

void get_vulkan_extensions(u32& out_extension_count, ReiToEngine::List<const char*>& out_extensions);
b8 get_vulkan_validation_layers(u32& out_layer_count, ReiToEngine::List<const char*>& out_layers);

b8 create_debugger(VkInstance& instance, VkAllocationCallbacks*& allocator, VkDebugUtilsMessengerEXT& debug_messenger);

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,VkDebugUtilsMessageTypeFlagsEXT              messageTypes,const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData, void* pUserData);

b8 VulkanRenderBackend::Initialize(ERenderBackendType renderer_type, const char* application_name, PlatformState* plat_state) {
    allocator = nullptr;

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = application_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "ReiToEngine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    ReiToEngine::List<const char*>extensions;
    u32 extension_count;
    get_vulkan_extensions(extension_count, extensions);
    create_info.enabledExtensionCount = extension_count;
    create_info.ppEnabledExtensionNames = extensions.data();

    ReiToEngine::List<const char*> validation_layers;
    u32 validation_layer_count = 0;
    get_vulkan_validation_layers(validation_layer_count, validation_layers);

    create_info.enabledLayerCount = validation_layer_count;
    create_info.ppEnabledLayerNames = validation_layers.data();

    // compactibility for MoltenVK on macOS
    // mostly no affect on other platforms
    // #ifdef RT_SYSTEM_APPLE
    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    // #endif
    RT_VK_CHECK(vkCreateInstance(&create_info, allocator, &instance));
    debugger_enabled = create_debugger(instance, allocator, debug_messenger);

    vulkan_initalize_physical_devices(instance, devices);

    RT_LOG_INFO("Vulkan Instance created successfully.");
    return true;
}
b8 VulkanRenderBackend::Terminate(){
    for (auto& swapchain : swapchains) {
        vulkan_swapchain_destroy({instance, allocator}, swapchain);

        if (swapchain.surface != VK_NULL_HANDLE)
        {
            PlatformDestroyVulkanSurface(instance, swapchain.surface);
            swapchain.surface = VK_NULL_HANDLE;
        }

    }
    swapchains.clear();

    for (auto& device : devices) {
        vulkan_logical_device_destroy(device);
        device.is_inused = false;
        device.logical_device = VK_NULL_HANDLE;
    }

    if (debugger_enabled) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        RT_ASSERT_MESSAGE(func != nullptr, "Failed to get vkDestroyDebugUtilsMessengerEXT function.");
        func(instance, debug_messenger, allocator);
        RT_LOG_DEBUG("Vulkan Debug Messenger destroyed.");
        debug_messenger = VK_NULL_HANDLE;
    }
    RT_LOG_INFO("Destroying Vulkan Instance.");
    vkDestroyInstance(instance, allocator);
    return true;
}
b8 VulkanRenderBackend::Tick(){return true;}
b8 VulkanRenderBackend::Resized(u32 width, u32 height){return true;}

b8 VulkanRenderBackend::BeginFrame(f64 delta_time){return true;}
b8 VulkanRenderBackend::EndFrame(f64 delta_time){return true;}

b8 VulkanRenderBackend::CreateSurface(RT_Platform_State& platform_state, SurfaceDesc& desc)
{
    RT_LOG_INFO("CreateSurface");

    swapchains.emplace_back();
    VulkanSwapchainContext& swapchain = swapchains.back();
    swapchain.width = desc.width;
    swapchain.height = desc.height;

    swapchain_map[desc.p_window] = &swapchain;

    PlatformCreateVulkanSurface(platform_state, desc, instance, swapchain.surface);

    swapchain.requirements = {};
    swapchain.requirements.queue_families[VulkanQueueFamilyIndicesType::GRAPHICS] = true;
    swapchain.requirements.queue_families[VulkanQueueFamilyIndicesType::PRESENT] = true;
    swapchain.requirements.queue_families[VulkanQueueFamilyIndicesType::TRANSFER] = true;
    swapchain.requirements.queue_families[VulkanQueueFamilyIndicesType::COMPUTE] = true;
    swapchain.requirements.sample_anisotropy = true;
    swapchain.requirements.discrete_gpu = true;

    platform_get_required_vulkan_extensions(swapchain.requirements.required_extensions);

    swapchain.requirements.required_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    swapchain.queue_family_indices.clear();
    swapchain.device_combination = nullptr;
    swapchain.queue_family_indices.clear();

    vulkan_physical_device_select(instance, swapchain, devices);

    vulkan_logical_device_create(swapchain);
    // device->is_inused = true;
    // if(!inused_devices.contains(device)) {
        // inused_devices.insert(device);
    // }
    // swapchain.selected_physical_device = device;

    vulkan_swapchain_create({instance, allocator}, swapchain);
    return true;
}
}
