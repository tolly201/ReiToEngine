#include "VulkanRendererBackend.h"
#include "L20_Platform/Include.h"
#include "VulkanExtensions.h"
#include "VulkanDevices.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFrameBuffer.h"
#include "VulkanFence.h"
#include "VulkanObjectShader.h"
namespace ReiToEngine
{

void get_vulkan_extensions(u32& out_extension_count, ReiToEngine::List<const char*>& out_extensions);
b8 get_vulkan_validation_layers(u32& out_layer_count, ReiToEngine::List<const char*>& out_layers);

b8 create_debugger(VkInstance& instance, VkAllocationCallbacks*& allocator, VkDebugUtilsMessengerEXT& debug_messenger);

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,VkDebugUtilsMessageTypeFlagsEXT              messageTypes,const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData, void* pUserData);

b8 create_buffers(VulkanContextRef context);

void create_command_buffers(VulkanSwapchainContext& swapchain);
void regenerate_frame_buffers(VulkanContextRef context, VulkanSwapchainContext& swapchain, VulkanRenderPass& render_pass);
b8 recreate_swapchain(VulkanContextRef context, VulkanSwapchainContext& swapchain);

static b8 rebuild_swapchain_pipeline(VulkanContextRef context, VulkanSwapchainContext& swapchain) {
    // Destroy old framebuffers first
    for (auto& fb : swapchain.framebuffers) {
        vulkan_frame_buffer_destroy(context, swapchain, fb);
    }
    swapchain.framebuffers.clear();

    // Destroy old swapchain and depth image
    vulkan_swapchain_destroy(context, swapchain);

    // Recreate swapchain
    vulkan_swapchain_create(context, swapchain);

    // Update render pass extents
    swapchain.render_pass.width = swapchain.width;
    swapchain.render_pass.height = swapchain.height;

    // Recreate framebuffers matching new images and depth
    regenerate_frame_buffers(context, swapchain, swapchain.render_pass);

    // Recreate/resize command buffers to match image count
    create_command_buffers(swapchain);

    return true;
}

b8 VulkanRenderBackend::Initialize([[maybe_unused]]ERenderBackendType renderer_type, const char* application_name,[[maybe_unused]] PlatformState* plat_state) {
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

    RT_LOG_INFO_PLATFORM("Vulkan Instance created successfully.");
    return true;
}
b8 VulkanRenderBackend::Terminate(){
    for (auto& device : devices) {
        if (device.logical_device != VK_NULL_HANDLE)
        vkDeviceWaitIdle(device.logical_device);
    }

    for (auto& swapchain : swapchains) {
        vulkan_swapchain_destroy({instance, allocator}, swapchain);

        for (u32 i = 0; i < swapchain.max_frames_in_flight; ++i)
        {
            vkDestroySemaphore(swapchain.device_combination->logical_device, swapchain.image_available_semaphores[i], allocator);
            vkDestroySemaphore(swapchain.device_combination->logical_device, swapchain.render_finished_semaphores[i], allocator);
            vulkan_fence_destroy({instance, allocator}, swapchain.device_combination->logical_device, swapchain.in_flight_fences[i]);
        }
        swapchain.image_available_semaphores.clear();
        swapchain.render_finished_semaphores.clear();
        swapchain.in_flight_fences.clear();
        swapchain.images_in_flight.clear();
        swapchain.in_flight_fence_count = 0;


        for (auto& frame_buffer : swapchain.framebuffers) {
            vulkan_frame_buffer_destroy({instance, allocator}, swapchain, frame_buffer);
        }
        swapchain.framebuffers.clear();

        if (swapchain.render_pass.handle != VK_NULL_HANDLE) {
            vulkan_renderpass_destroy({instance, allocator}, swapchain, swapchain.render_pass);
            swapchain.render_pass.handle = VK_NULL_HANDLE;
        }
        if (swapchain.surface != VK_NULL_HANDLE)
        {
            PlatformDestroyVulkanSurface(instance, swapchain.surface);
            swapchain.surface = VK_NULL_HANDLE;
        }
    }
    swapchains.clear();

    for (auto& device : devices) {
        vulkan_logical_device_destroy({instance, allocator}, device);
        device.is_inused = false;
        device.logical_device = VK_NULL_HANDLE;
    }

    if (debugger_enabled) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        RT_ASSERT_MESSAGE_PLATFORM(func != nullptr, "Failed to get vkDestroyDebugUtilsMessengerEXT function.");
        func(instance, debug_messenger, allocator);
        RT_LOG_DEBUG_PLATFORM("Vulkan Debug Messenger destroyed.");
        debug_messenger = VK_NULL_HANDLE;
    }
    RT_LOG_INFO_PLATFORM("Destroying Vulkan Instance.");
    vkDestroyInstance(instance, allocator);
    return true;
}
b8 VulkanRenderBackend::Tick(){return true;}
b8 VulkanRenderBackend::Resized(SurfaceDesc& desc, u32 width, u32 height){
    VulkanSwapchainContext* swapchain = swapchain_map[desc.p_window];
    if(swapchain == nullptr) {
        RT_LOG_ERROR_PLATFORM("Failed to find swapchain for window during resize.");
        return false;
    }

    swapchain->width = width;
    swapchain->height = height;

    swapchain->frame_size_generation++;

    return true;
}

b8 VulkanRenderBackend::BeginFrame([[maybe_unused]]f64 delta_time){
    for (auto& device : devices) {
        if (device.is_inused)
        {
            if (vkDeviceWaitIdle(device.logical_device) != VK_SUCCESS) {
                RT_LOG_ERROR_FMT_PLATFORM("Failed to wait device idle for device {}.", device.device_properties.properties.deviceName);
                return false;
            }
        };
    }

    for (auto& swapchain : swapchains) {
        if (swapchain.recreating_swapchain)
        {
            VkResult result = vkDeviceWaitIdle(swapchain.device_combination->logical_device);
            if (result != VK_SUCCESS) {
                RT_LOG_ERROR_FMT_PLATFORM("Failed to wait device idle before rebuilding swapchain {}.", swapchain.index);
                return false;
            }
            if (!rebuild_swapchain_pipeline({instance, allocator}, swapchain)) {
                RT_LOG_ERROR_FMT_PLATFORM("Failed to rebuild swapchain {}.", swapchain.index);
                return false;
            }
            swapchain.recreating_swapchain = false;
            swapchain.last_frame_size_generation = swapchain.frame_size_generation;
            continue;
        };


        if (swapchain.frame_size_generation != swapchain.last_frame_size_generation) {
            VkResult result = vkDeviceWaitIdle(swapchain.device_combination->logical_device);
            if (result != VK_SUCCESS) {
                RT_LOG_ERROR_FMT_PLATFORM("Failed to wait device idle before recreating swapchain {}.", swapchain.index);
                return false;
            }
            swapchain.recreating_swapchain = true;
            RT_LOG_INFO_FMT_PLATFORM("Marked swapchain {} for recreation.", swapchain.index);
            continue; // handle on next loop
        }


        if (!vulkan_fence_wait(swapchain.device_combination->logical_device, swapchain.in_flight_fences[swapchain.current_frame], UINT64_MAX)) {
            RT_LOG_ERROR_FMT_PLATFORM("Failed to wait for in-flight fence for swapchain {}.", swapchain.index);
            return false;
        }

        if (!vulkan_swapchain_acquire_next_image_index({instance, allocator}, swapchain, UINT32_MAX, swapchain.image_available_semaphores[swapchain.current_frame], VK_NULL_HANDLE, swapchain.current_image_index)) {
            if (swapchain.recreating_swapchain) {
                RT_LOG_INFO_FMT_PLATFORM("Acquire indicated out-of-date for swapchain {}. Marked for rebuild.", swapchain.index);
                continue; // handle rebuild next loop
            }
            RT_LOG_ERROR_FMT_PLATFORM("Failed to acquire next image index for swapchain {}.", swapchain.index);
            return false;
        }

        VulkanCommandBuffer& command_buffer = swapchain.device_combination->command_buffers[VulkanQueueFamilyIndicesType::GRAPHICS][swapchain.current_image_index];
        vulkan_command_buffer_reset(command_buffer);
        vulkan_command_buffer_begin(command_buffer, false, false, false);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = (f32)swapchain.height;
        viewport.width = (f32)swapchain.width;
        viewport.height = -(f32)swapchain.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        scissor.extent.width = swapchain.width;
        scissor.extent.height = swapchain.height;


        vkCmdSetViewport(command_buffer.command_buffer, 0, 1, &viewport);
        vkCmdSetScissor(command_buffer.command_buffer, 0, 1, &scissor);

        vulkan_renderpass_begin({instance, allocator}, swapchain, swapchain.render_pass, command_buffer, swapchain.framebuffers[swapchain.current_image_index].handle);
    }

    return true;
}
b8 VulkanRenderBackend::EndFrame([[maybe_unused]]f64 delta_time){
    for (auto& swapchain : swapchains) {
        VulkanCommandBuffer& command_buffer = swapchain.device_combination->command_buffers[VulkanQueueFamilyIndicesType::GRAPHICS][swapchain.current_image_index];
        vulkan_renderpass_end({instance, allocator}, swapchain, swapchain.render_pass, command_buffer, swapchain.framebuffers[swapchain.current_image_index].handle);
        vulkan_command_buffer_end(command_buffer);

        if (swapchain.images_in_flight[swapchain.current_image_index] != VK_NULL_HANDLE) {
            vulkan_fence_wait(swapchain.device_combination->logical_device, *swapchain.images_in_flight[swapchain.current_image_index], UINT64_MAX);
        }
        swapchain.images_in_flight[swapchain.current_image_index] = &swapchain.in_flight_fences[swapchain.current_frame];

        vulkan_fence_reset(swapchain.device_combination->logical_device, swapchain.in_flight_fences[swapchain.current_frame]);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer.command_buffer;

        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &swapchain.image_available_semaphores[swapchain.current_frame];

        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &swapchain.render_finished_semaphores[swapchain.current_frame];

        VkPipelineStageFlags wait_stages[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submit_info.pWaitDstStageMask = wait_stages;

        VkResult result = vkQueueSubmit(swapchain.device_combination->queues[VulkanQueueFamilyIndicesType::GRAPHICS], 1, &submit_info, swapchain.in_flight_fences[swapchain.current_frame].handle);
        if (result != VK_SUCCESS) {
            RT_LOG_ERROR_FMT_PLATFORM("Failed to submit draw command buffer for swapchain {}.", swapchain.index);
            return false;
        }

        VkQueue graphics_queue = swapchain.device_combination->queues[VulkanQueueFamilyIndicesType::GRAPHICS];
        VkQueue present_queue = graphics_queue;
        if (swapchain.device_combination->queues.contains(VulkanQueueFamilyIndicesType::PRESENT)) {
            present_queue = swapchain.device_combination->queues[VulkanQueueFamilyIndicesType::PRESENT];
        }

        // 将当前帧的 render_finished_semaphore 作为 present 的等待信号量
        if (!vulkan_swapchain_present(
                {instance, allocator},
                swapchain,
                graphics_queue,
                present_queue,
                swapchain.render_finished_semaphores[swapchain.current_frame],
                swapchain.current_image_index)) {
            RT_LOG_ERROR_FMT_PLATFORM("Present reported suboptimal/out-of-date for swapchain {}. Will rebuild.", swapchain.index);
            swapchain.recreating_swapchain = true;
            // Let the next frame handle rebuild
        }

        vulkan_command_buffer_update_submmitted(command_buffer);
    }

    return true;
}

b8 VulkanRenderBackend::CreateSurface(RT_Platform_State& platform_state, SurfaceDesc& desc)
{
    RT_LOG_INFO_PLATFORM("CreateSurface");
    RT_LOG_DEBUG_FMT_PLATFORM("width: {}, height: {}", desc.width, desc.height);

    swapchains.emplace_back();
    VulkanSwapchainContext& swapchain = swapchains.back();
    swapchain.width = desc.width;
    swapchain.height = desc.height;
    swapchain.frame_size_generation = 0;
    swapchain.last_frame_size_generation = 0;
    swapchain.recreating_swapchain = false;
    swapchain.current_frame = 0;
    swapchain.index = (u32)swapchains.size() - 1;
    swapchain.p_window = desc.p_window;

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
    swapchain.device_combination = nullptr;
    swapchain.queue_family_indices.clear();

    vulkan_physical_device_select(instance, swapchain, devices);

    vulkan_logical_device_create({instance, allocator}, swapchain);
    // device->is_inused = true;
    // if(!inused_devices.contains(device)) {
        // inused_devices.insert(device);
    // }
    // swapchain.selected_physical_device = device;

    vulkan_swapchain_create({instance, allocator}, swapchain);

    vulkan_renderpass_create({instance, allocator}, swapchain, swapchain.render_pass);

    vulkan_object_shader_create({instance, allocator, swapchain.device_combination, &swapchain}, "Builtin.ObjectShader", swapchain.device_combination->shader_sets[0]);

    regenerate_frame_buffers({instance, allocator}, swapchain, swapchain.render_pass);

    create_command_buffers(swapchain);

    swapchain.image_available_semaphores.resize(swapchain.max_frames_in_flight);
    swapchain.render_finished_semaphores.resize(swapchain.max_frames_in_flight);
    swapchain.in_flight_fences.resize(swapchain.max_frames_in_flight);

    for (u32 i = 0; i < swapchain.max_frames_in_flight; ++i)
    {
        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphore_info.pNext = nullptr;
        semaphore_info.flags = 0;

        vkCreateSemaphore(swapchain.device_combination->logical_device, &semaphore_info, allocator, &swapchain.image_available_semaphores[i]);
        vkCreateSemaphore(swapchain.device_combination->logical_device, &semaphore_info, allocator, &swapchain.render_finished_semaphores[i]);

        vulkan_fence_create({instance, allocator},swapchain.device_combination->logical_device, true, swapchain.in_flight_fences[i]);
    }

    swapchain.images_in_flight.resize(swapchain.image_count, nullptr);

    create_buffers({instance, allocator, swapchain.device_combination, &swapchain});

    return true;
}

void create_command_buffers(VulkanSwapchainContext& swapchain)
{
    List<VulkanCommandBuffer>& command_buffers = swapchain.device_combination->command_buffers[VulkanQueueFamilyIndicesType::GRAPHICS];
    // Free existing if present, then resize to match current image count
    for (auto& cb : command_buffers) {
        if (cb.state != VulkanCommandBufferState::NOT_ALLOCATED) {
            vulkan_command_buffer_free(swapchain.device_combination->logical_device, swapchain.device_combination->command_pools[VulkanQueueFamilyIndicesType::GRAPHICS], cb);
        }
    }
    command_buffers.clear();
    command_buffers.resize(swapchain.image_count);

    for (u32 i = 0; i < swapchain.image_count; ++i)
    {
        vulkan_command_buffer_allocate(swapchain.device_combination->logical_device, swapchain.device_combination->command_pools[VulkanQueueFamilyIndicesType::GRAPHICS], true, command_buffers[i]);

    }
}

void regenerate_frame_buffers(VulkanContextRef context, VulkanSwapchainContext& swapchain, VulkanRenderPass& render_pass)
{
    // 销毁旧的 framebuffer
    for (auto& fb : swapchain.framebuffers) {
        vulkan_frame_buffer_destroy(context, swapchain, fb);
    }
    swapchain.framebuffers.clear();
    swapchain.framebuffers.resize(swapchain.images.size());

    for (u32 i = 0; i < swapchain.images.size(); ++i)
    {
        u32 attachment_count = 2;
        VkImageView attachments[2];
        attachments[0] = swapchain.images[i].view; // 颜色附件
        attachments[1] = swapchain.depth_image.view; // 深度附件

        vulkan_frame_buffer_create(context, swapchain, render_pass, swapchain.width, swapchain.height, attachment_count, attachments, swapchain.framebuffers[i]);
    }
}

b8 create_buffers(VulkanContextRef context)
{
    return true;
}
} // namespace ReiToEngine
