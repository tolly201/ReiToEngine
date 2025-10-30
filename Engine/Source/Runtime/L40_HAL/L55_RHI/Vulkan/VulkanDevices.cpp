#include "VulkanDevices.h"
#include "VulkanCommandBuffer.h"
#include "VulkanObjectShader.h"
#include "VulkanBuffer.h"
namespace ReiToEngine
{
b8 vulkan_initalize_physical_devices(VkInstance& instance, List<VulkanDeviceCombination>& out_physical_devices)
{
    u32 device_count = 0;
    RT_VK_CHECK(vkEnumeratePhysicalDevices(instance, &device_count, nullptr));
    if (device_count == 0) {
        RT_LOG_FATAL_PLATFORM("Failed to find GPUs with Vulkan support.");
        return false;
    }

    List<VkPhysicalDevice> physical_devices;
    physical_devices.resize(device_count);
    RT_VK_CHECK(vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data()));

    for (u32 i = 0; i < device_count; ++i) {
        VulkanDeviceCombination device_combination{};

        device_combination.physical_device = physical_devices[i];
        device_combination.logical_device = VK_NULL_HANDLE;
        device_combination.is_inused = false;

        device_combination.device_properties = {};
        device_combination.device_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;

        vkGetPhysicalDeviceProperties2(physical_devices[i], &device_combination.device_properties);

        device_combination.device_features = {};
        device_combination.device_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        vkGetPhysicalDeviceFeatures2(physical_devices[i], &device_combination.device_features);

        device_combination.memory_properties = {};
        device_combination.memory_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
        vkGetPhysicalDeviceMemoryProperties2(physical_devices[i], &device_combination.memory_properties);

        u32 queue_family_count = 0;

        vkGetPhysicalDeviceQueueFamilyProperties2(device_combination.physical_device, &queue_family_count, nullptr);

        device_combination.queue_families.clear();
        device_combination.queue_families.resize(queue_family_count);

        for (u32 q = 0; q < queue_family_count; ++q) {
            device_combination.queue_families[q].sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        }

        vkGetPhysicalDeviceQueueFamilyProperties2(device_combination.physical_device, &queue_family_count, device_combination.queue_families.data());

        device_combination.available_queue_family_indices.clear();
        device_combination.dedicated_queue_family_indices.clear();
        device_combination.inuse_queue_family_indices.clear();

        for (u32 q = 0; q < queue_family_count; ++q) {
            const auto& qf = device_combination.queue_families[q].queueFamilyProperties;
            const VkQueueFlags f = qf.queueFlags;

            if (f & VK_QUEUE_GRAPHICS_BIT) {
                device_combination.available_queue_family_indices[VulkanQueueFamilyIndicesType::GRAPHICS].push_back((i32)q);

                if ((f & (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT)) == 0) {
                    device_combination.dedicated_queue_family_indices[VulkanQueueFamilyIndicesType::GRAPHICS].push_back((i32)q);
                }
            }
            if (f & VK_QUEUE_COMPUTE_BIT) {
                device_combination.available_queue_family_indices[VulkanQueueFamilyIndicesType::COMPUTE].push_back((i32)q);
                if ((f & VK_QUEUE_GRAPHICS_BIT) == 0) {
                    device_combination.dedicated_queue_family_indices[VulkanQueueFamilyIndicesType::COMPUTE].push_back((i32)q);
                }
            }
            if (f & VK_QUEUE_TRANSFER_BIT) {
                device_combination.available_queue_family_indices[VulkanQueueFamilyIndicesType::TRANSFER].push_back((i32)q);
                // “专用传输”常用定义：只有 TRANSFER（无 GRAPHICS/COMPUTE）
                if ((f & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) == 0) {
                    device_combination.dedicated_queue_family_indices[VulkanQueueFamilyIndicesType::TRANSFER].push_back((i32)q);
                }
            }
        }

        u32 extension_count = 0;
        RT_VK_CHECK(vkEnumerateDeviceExtensionProperties(physical_devices[i], nullptr, &extension_count, nullptr));
        device_combination.supported_extensions.resize(extension_count);
        RT_VK_CHECK(vkEnumerateDeviceExtensionProperties(physical_devices[i], nullptr, &extension_count, device_combination.supported_extensions.data()));

        out_physical_devices.push_back(std::move(device_combination));

        RT_LOG_INFO_FMT_PLATFORM("Found physical device: {}.", std::string(device_combination.device_properties.properties.deviceName));

        RT_LOG_INFO_FMT_PLATFORM("VendorID: {}, DeviceID: {}.", device_combination.device_properties.properties.vendorID, device_combination.device_properties.properties.deviceID);
        RT_LOG_INFO_FMT_PLATFORM("Type: {}.", (device_combination.device_properties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) ? "Discrete" : "Integrated");
        RT_LOG_INFO_FMT_PLATFORM("API Version: {}.{}.{}.",
        VK_VERSION_MAJOR(device_combination.device_properties.properties.apiVersion),
        VK_VERSION_MINOR(device_combination.device_properties.properties.apiVersion),
        VK_VERSION_PATCH(device_combination.device_properties.properties.apiVersion));
        RT_LOG_INFO_FMT_PLATFORM("Driver Version: {}.", device_combination.device_properties.properties.driverVersion);
        RT_LOG_INFO_FMT_PLATFORM("Max Memory: {} MB.", device_combination.memory_properties.memoryProperties.memoryHeaps[0].size / (1024 * 1024));

        // 打印支持的扩展
        RT_LOG_INFO_PLATFORM("Supported extensions:");
        for (const auto& ext : device_combination.supported_extensions) {
            RT_LOG_INFO_FMT_PLATFORM("{}", std::string(ext.extensionName));
        }

        // 打印队列族信息
        RT_LOG_INFO_PLATFORM("Queue Families:");
        for (u32 q = 0; q < device_combination.queue_families.size(); ++q) {
            RT_LOG_INFO_FMT_PLATFORM("[{}] Flags: {}, Count: {}", q, device_combination.queue_families[q].queueFamilyProperties.queueFlags, device_combination.queue_families[q].queueFamilyProperties.queueCount);
        }
    }
    return true;
}

b8 select_physical_device(VkInstance& instance, VulkanSwapchainContext& swapchain_content, List<VulkanDeviceCombination>&, VulkanDeviceCombination*&);
u32 physical_device_meets_requirements(VulkanDeviceCombination&, VulkanSwapchainContext&);
b8 vulkan_physical_device_select(VkInstance& instance, VulkanSwapchainContext& swapchain_content, List<VulkanDeviceCombination>& devices)
{
    RT_LOG_DEBUG_PLATFORM("START SELECT PHYSICAL DEVICE:");
    VulkanDeviceCombination* device_combination = nullptr;
    select_physical_device(instance, swapchain_content, devices, device_combination);
    if (device_combination == nullptr) {
        RT_LOG_FATAL_PLATFORM("Failed to find a suitable GPU for the swapchain.");
        return false;
    }
    swapchain_content.device_combination = const_cast<VulkanDeviceCombination*>(device_combination);

    for (u8 i = 0; i < static_cast<u8>(VulkanQueueFamilyIndicesType::MAX); ++i)
    {
        VulkanQueueFamilyIndicesType type = static_cast<VulkanQueueFamilyIndicesType>(i);
        RT_LOG_INFO_FMT_PLATFORM("Selected queue family index for type {}: {}", static_cast<u8>(type), swapchain_content.queue_family_indices[type]);
    }

    return true;
}
b8 vulkan_logical_device_create(VulkanContextRef ref, VulkanSwapchainContext& swapchain_context){
    VulkanDeviceCombination& dc = *swapchain_context.device_combination;
    if (dc.logical_device != VK_NULL_HANDLE) {
        // 已创建，直接复用
        return true;
    }

    // 1) 收集将要用到的队列族（去重）
    List<u32> families;
    families.clear();
    auto add_family = [&](uint32_t f) {
        if (f == static_cast<uint32_t>(-1)) return;
        for (auto v : families) if (v == f) return;
        families.push_back(f);
    };
    add_family(swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::GRAPHICS]);
    add_family(swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::PRESENT]);
    add_family(swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::COMPUTE]);
    add_family(swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::TRANSFER]);

    if (families.empty()) {
        RT_LOG_ERROR_PLATFORM("No queue families selected to create logical device.");
        return false;
    }

    RT_LOG_INFO_FMT_PLATFORM("Creating {} families.", families.size());


    // 2) 组装队列创建信息（每族先要 1 条队列）
    List<VkDeviceQueueCreateInfo> qcis;
    qcis.clear();

    List<List<float>> all_queue_priorities;
    all_queue_priorities.clear();

    u32 queue_count_per_family = 2;

    for (u32 family : families) {
        // 容错：若索引越界则跳过
        RT_LOG_DEBUG_FMT_PLATFORM("Creating queue for family index: {} / {}.", family, dc.queue_families.size());
        if (family >= dc.queue_families.size()) continue;
        const auto& qfp = dc.queue_families[family].queueFamilyProperties;

        RT_LOG_DEBUG_FMT_PLATFORM("Creating queue for family index Priority: {}.", qfp.queueCount);

        if (qfp.queueCount == 0) continue;

        RT_LOG_DEBUG_FMT_PLATFORM("Create queue for family index Priority: {}.", family);
        VkDeviceQueueCreateInfo qci{};
        qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qci.queueFamilyIndex = family;
        qci.queueCount = RTMIN(queue_count_per_family, qfp.queueCount);
        all_queue_priorities.emplace_back();
        List<float>& queue_priorities = all_queue_priorities.back();
        queue_priorities.clear();
        queue_priorities.resize(qci.queueCount, 1.0f);

        qci.pQueuePriorities = queue_priorities.data();
        qcis.push_back(qci);
    }

    if (qcis.empty()) {
        RT_LOG_ERROR_PLATFORM("No valid queue families to create device queues.");
        return false;
    }
    else
    {
        RT_LOG_INFO_FMT_PLATFORM("Creating {} device queues.", qcis.size());
    }

    VkPhysicalDeviceFeatures enabled{};
    if (swapchain_context.requirements.sample_anisotropy &&
        dc.device_features.features.samplerAnisotropy == VK_TRUE) {
        enabled.samplerAnisotropy = VK_TRUE;
    }

    // 5) 创建逻辑设备
    VkDeviceCreateInfo dci{};
    dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    dci.queueCreateInfoCount = static_cast<uint32_t>(qcis.size());
    dci.pQueueCreateInfos = qcis.data();
    dci.enabledExtensionCount = static_cast<uint32_t>(swapchain_context.requirements.required_extensions.size());
    dci.ppEnabledExtensionNames = swapchain_context.requirements.required_extensions.data();
    dci.pEnabledFeatures = &enabled; // 如需 Features2/链，可改用 pNext

    if (!RT_VK_CHECK(vkCreateDevice(dc.physical_device, &dci, ref.allocator, &dc.logical_device))) {
        RT_LOG_ERROR_PLATFORM("vkCreateDevice failed.");
        dc.logical_device = VK_NULL_HANDLE;
        return false;
    }

    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::GRAPHICS];

    dc.command_pools[VulkanQueueFamilyIndicesType::GRAPHICS] = VK_NULL_HANDLE;

    RT_VK_CHECK(vkCreateCommandPool(dc.logical_device, &pool_info, nullptr, &dc.command_pools[VulkanQueueFamilyIndicesType::GRAPHICS]));

    dc.is_inused = true;

    // 如需获取队列句柄，可在此使用 vkGetDeviceQueue(dc.logical_device, family, 0, &queue)
    // 并存入你的 SwapchainContext 或设备级缓存。

    for (const auto& [type, family_index] : swapchain_context.queue_family_indices) {
        if (family_index != static_cast<i32>(-1)) {
            VkQueue queue = VK_NULL_HANDLE;
            vkGetDeviceQueue(dc.logical_device, family_index, 0, &queue);
            dc.queues[type] = queue;
        }
    }

    dc.shader_sets.clear();
    dc.shader_sets.resize(1);

    VkDescriptorSetLayoutBinding global_ubo_layout_binding{};
    global_ubo_layout_binding.binding = 0;
    global_ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    global_ubo_layout_binding.descriptorCount = 1;
    global_ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    global_ubo_layout_binding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo global_layout_info{};
    global_layout_info.bindingCount = 1;
    global_layout_info.pBindings = &global_ubo_layout_binding;
    global_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    RT_VK_CHECK(vkCreateDescriptorSetLayout(dc.logical_device, &global_layout_info, ref.allocator, &dc.set0_global_layout));

    RT_VK_CHECK(vkCreateDescriptorSetLayout(dc.logical_device, &global_layout_info, ref.allocator, &dc.set1_material_layout));
    RT_VK_CHECK(vkCreateDescriptorSetLayout(dc.logical_device, &global_layout_info, ref.allocator, &dc.set2_object_layout));

    RT_LOG_INFO_PLATFORM("Logical device created.");
    return true;
}
b8 vulkan_physical_device_destroy();
b8 vulkan_logical_device_destroy(VulkanContextRef ref, VulkanDeviceCombination& dc)
{
    vkDestroyDescriptorSetLayout(dc.logical_device, dc.set0_global_layout, ref.allocator);
    vkDestroyDescriptorSetLayout(dc.logical_device, dc.set1_material_layout, ref.allocator);
    vkDestroyDescriptorSetLayout(dc.logical_device, dc.set2_object_layout, ref.allocator);

    vulkan_buffer_destroy({ref.instance, ref.allocator, &dc}, dc.vertex_buffer);
    vulkan_buffer_destroy({ref.instance, ref.allocator, &dc}, dc.index_buffer);

    for (VulkanShaderSet& shader_set : dc.shader_sets) {
        vulkan_object_shader_destroy(ref, shader_set);
    }

    for (auto& [type, buffers] : dc.command_buffers) {
        for (auto& cb : buffers) {
            if (cb.handle != VK_NULL_HANDLE) {
                vulkan_command_buffer_free(dc.logical_device, dc.command_pools[type], cb);
            }
        }
    }
    dc.command_buffers.clear();

    for (auto& [type, pool] : dc.command_pools) {
        if (pool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(dc.logical_device, pool, nullptr);
            pool = VK_NULL_HANDLE;
        }
    }

    if (dc.logical_device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(dc.logical_device);
        vkDestroyDevice(dc.logical_device, ref.allocator);
        dc.logical_device = VK_NULL_HANDLE;
        dc.is_inused = false;
        dc.inuse_queue_family_indices.clear();
        RT_LOG_INFO_PLATFORM("Logical device destroyed.");
    }
    return true;
}

b8 vulkan_device_detect_depth_format(VulkanDeviceCombination& device_combination)
{
    // 常用深度格式列表，按优先级从高到低

    List<VkFormat> depth_formats;
    depth_formats.clear();

    depth_formats.push_back(VK_FORMAT_D32_SFLOAT_S8_UINT);
    depth_formats.push_back(VK_FORMAT_D32_SFLOAT);
    depth_formats.push_back(VK_FORMAT_D24_UNORM_S8_UINT);
    depth_formats.push_back(VK_FORMAT_D16_UNORM_S8_UINT);
    depth_formats.push_back(VK_FORMAT_D16_UNORM);

    u32 flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    for (VkFormat format : depth_formats) {
        VkFormatProperties2 props2;
        VkFormatProperties3 props3;
        props2.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
        props2.pNext = &props3;

        props3.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3;
        props3.pNext = nullptr;

        vkGetPhysicalDeviceFormatProperties2(device_combination.physical_device, format, &props2);

        // 检查是否支持作为深度/模板附件使用
        if ((props3.linearTilingFeatures & flags) == flags) {
            device_combination.depth_format = format;
            RT_LOG_INFO_FMT_PLATFORM("Selected depth format: {}", static_cast<u32>(format));
            return true;
        }

        else if ((props3.optimalTilingFeatures & flags) == flags) {
            device_combination.depth_format = format;
            RT_LOG_INFO_FMT_PLATFORM("Selected depth format: {}", static_cast<u32>(format));
            return true;
        }
    }

    RT_LOG_ERROR_PLATFORM("Failed to find a supported depth format.");
    return false;
}

b8 select_physical_device([[maybe_unused]]VkInstance& instance, VulkanSwapchainContext& swapchain_content,  List<VulkanDeviceCombination>& devices, VulkanDeviceCombination*& out_device)
{
    RT_LOG_DEBUG_FMT_PLATFORM("START SELECT DEVICE: {}", devices.size());

    u32 highest_score = 0;

    for (u32 i = 0; i < devices.size(); ++i) {
        VulkanDeviceCombination& device_combination = devices[i];


        RT_LOG_DEBUG_FMT_PLATFORM("Querying swapchain support for device: {}", i);
        u32 score = physical_device_meets_requirements(device_combination, swapchain_content);

        if (score > highest_score) {
            highest_score = score;
            out_device = &device_combination;
            RT_LOG_DEBUG_FMT_PLATFORM("New best device found: {} with score {}.", i, score);
        }
    }

    return out_device != nullptr;
}

u32 physical_device_meets_requirements(VulkanDeviceCombination& device_combination, VulkanSwapchainContext& swapchain_context)
{
    RT_LOG_DEBUG_PLATFORM("START evaluating physical device:");

    swapchain_context.queue_family_indices.clear();

    VulkanPhysicalDeviceRequirements& requirements = swapchain_context.requirements;


    if (requirements.discrete_gpu)
    {
        if(device_combination.device_properties.properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            RT_LOG_WARN_PLATFORM("Physical device is not a discrete GPU.");
            return 0;
        }
    }

    RT_LOG_INFO_PLATFORM("Graphics | Present | Compute | Transfer | Name | Flags");
    [[maybe_unused]]u8 min_transfer_score = 255;

    for (u8 i = 0; i < static_cast<u8>(VulkanQueueFamilyIndicesType::MAX); ++i)
    {
        VulkanQueueFamilyIndicesType type = static_cast<VulkanQueueFamilyIndicesType>(i);
        RT_LOG_DEBUG_FMT_PLATFORM("Checking queue family type: {}", static_cast<u8>(type));

        if (type == VulkanQueueFamilyIndicesType::PRESENT)
        {
            continue;
        }

        if (requirements.queue_families[type])
        {
            if (device_combination.dedicated_queue_family_indices.find(type) == device_combination.dedicated_queue_family_indices.end()
            && device_combination.available_queue_family_indices.find(type) == device_combination.available_queue_family_indices.end())
            {
                RT_LOG_WARN_PLATFORM("Physical device misses required queue family. Index:", static_cast<u8>(type));
                return 0;
            }

            if (device_combination.dedicated_queue_family_indices.find(type) != device_combination.dedicated_queue_family_indices.end())
            {
                swapchain_context.queue_family_indices[type] = device_combination.dedicated_queue_family_indices[type].front();
            }
            else
            {
                swapchain_context.queue_family_indices[type] = device_combination.available_queue_family_indices.at(type).front();
            }
        }
    }

    for (u32 i = 0; i < device_combination.queue_families.size(); ++i)
    {
        VkBool32 present_support = false;
        RT_VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device_combination.physical_device, i, swapchain_context.surface, &present_support));
        if (present_support && swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::PRESENT] == static_cast<i32>(-1)) {
            swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::PRESENT] = i;
            break;
        }
    }

    const bool ok_graphics = !requirements.queue_families[VulkanQueueFamilyIndicesType::GRAPHICS] || (swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::GRAPHICS] != static_cast<i32>(-1));
    const bool ok_present  = !requirements.queue_families[VulkanQueueFamilyIndicesType::PRESENT]  || (swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::PRESENT]  != static_cast<i32>(-1));
    const bool ok_compute  = !requirements.queue_families[VulkanQueueFamilyIndicesType::COMPUTE]  || (swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::COMPUTE]  != static_cast<i32>(-1));
    const bool ok_transfer = !requirements.queue_families[VulkanQueueFamilyIndicesType::TRANSFER] || (swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::TRANSFER] != static_cast<i32>(-1));

    if (!(ok_graphics && ok_present && ok_compute && ok_transfer)) {
        RT_LOG_WARN_PLATFORM("Physical device misses required queue family.");
        return 0;
    }

    if(requirements.required_extensions.size() > 0)
    {
        for (u32 i = 0; i < requirements.required_extensions.size(); ++i)
        {
            b8 extension_found = false;
            for (u32 j = 0; j < device_combination.supported_extensions.size(); ++j)
            {
                if (RT_CStringCompare(requirements.required_extensions[i], device_combination.supported_extensions[j].extensionName))
                {
                    extension_found = true;
                    break;
                }
            }

            if (!extension_found)
            {
                RT_LOG_WARN_FMT_PLATFORM("Physical device is missing required extension: {}", std::string(requirements.required_extensions[i]));
                return 0;
            }
        }
    }

    if (requirements.sample_anisotropy && !device_combination.device_features.features.samplerAnisotropy) {
        RT_LOG_WARN_PLATFORM("Physical device does not support sampler anisotropy.");
        return 0;
    }

    u32 score = 0;

    // 设备类型权重
    switch (device_combination.device_properties.properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:   score += 1000; break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: score += 500;  break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:    score += 300;  break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:            score += 100;  break;
        default:                                     score += 50;   break;
    }

    // 队列族独立性（独立传输/计算更佳）
    if (swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::TRANSFER] != static_cast<i32>(-1)) {
        score += (swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::TRANSFER] != swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::GRAPHICS]) ? 150 : 50;
    }
    if (swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::COMPUTE] != static_cast<i32>(-1)) {
        score += (swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::COMPUTE] != swapchain_context.queue_family_indices[VulkanQueueFamilyIndicesType::GRAPHICS]) ? 100 : 30;
    }

    {
        auto& queue_families = device_combination.queue_families;
        auto& out_indices = swapchain_context.queue_family_indices;
        const auto& gq = queue_families[out_indices[VulkanQueueFamilyIndicesType::GRAPHICS]].queueFamilyProperties;
        score += 10 * gq.queueCount;
        if (out_indices[VulkanQueueFamilyIndicesType::TRANSFER] != static_cast<i32>(-1)) {
            const auto& tq = queue_families[out_indices[VulkanQueueFamilyIndicesType::TRANSFER]].queueFamilyProperties;
            score += 5 * tq.queueCount;
        }
    }

    {
        const auto& memory_properties = device_combination.memory_properties;

        u64 device_local_bytes = 0;
        for (u32 h = 0; h < memory_properties.memoryProperties.memoryHeapCount; ++h) {
            const auto& heap = memory_properties.memoryProperties.memoryHeaps[h];
            if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                device_local_bytes += heap.size;
            }
        }
        u32 device_local_mb = static_cast<u32>(device_local_bytes / (1024 * 1024));
        score += device_local_mb / 512; // 每 512MB 加 1 分
    }

    {
        auto api = device_combination.device_properties.properties.apiVersion;
        score += 10 * VK_VERSION_MAJOR(api) + 2 * VK_VERSION_MINOR(api);
    }

    swapchain_context.swapchain_info = {};

    vulkan_device_query_swapchain_support(device_combination.physical_device, swapchain_context.surface, swapchain_context.swapchain_info);

    if (swapchain_context.swapchain_info.formats.size() <= 0 || swapchain_context.swapchain_info.present_modes.size() <= 0)
    {
        RT_LOG_WARN_PLATFORM("Physical device has no swapchain support.");
        return 0;
        }

    {
        bool has_srgb = false, has_bgra8 = false;
        for (u32 i = 0; i < swapchain_context.swapchain_info.formats.size(); ++i) {
            const auto& f = swapchain_context.swapchain_info.formats[i].surfaceFormat;
            if (f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) has_srgb = true;
            if (f.format == VK_FORMAT_B8G8R8A8_UNORM || f.format == VK_FORMAT_B8G8R8A8_SRGB) has_bgra8 = true;
        }
        if (has_srgb) score += 50;
        if (has_bgra8) score += 100;

        bool has_mailbox = false, has_immediate = false, has_fifo = false;
        for (u32 i = 0; i < swapchain_context.swapchain_info.present_modes.size(); ++i) {
            switch (swapchain_context.swapchain_info.present_modes[i]) {
                case VK_PRESENT_MODE_MAILBOX_KHR:  has_mailbox = true; break;
                case VK_PRESENT_MODE_IMMEDIATE_KHR:has_immediate = true; break;
                case VK_PRESENT_MODE_FIFO_KHR:     has_fifo = true; break;
                default: break;
            }
        }
        if (has_mailbox)  score += 60;
        if (has_immediate) score += 20;
        if (has_fifo)      score += 10;
    }

    // 特性偏好
    if (device_combination.device_features.features.samplerAnisotropy) score += 50;

    RT_LOG_INFO_FMT_PLATFORM("Score for device {}: {}", std::string(device_combination.device_properties.properties.deviceName), score);
    return score;
}

b8 vulkan_device_query_swapchain_support(VkPhysicalDevice& device, VkSurfaceKHR& surface, VulkanSwapChainSupportInfo& out_info)
{
    VkPhysicalDeviceSurfaceInfo2KHR surface_info{};
    surface_info.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surface_info.pNext = nullptr;
    surface_info.surface = surface;

    out_info.capabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
    out_info.capabilities.pNext = nullptr;

    u32 format_count = 0;

    RT_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilities2KHR(device, &surface_info, &out_info.capabilities));

    RT_VK_CHECK(vkGetPhysicalDeviceSurfaceFormats2KHR(device, &surface_info, &format_count, nullptr));

    out_info.formats.resize(format_count);
    for (u32 i = 0; i < format_count; ++i) {
        out_info.formats[i].sType = VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR;
        out_info.formats[i].pNext = nullptr;
    }
    RT_VK_CHECK(vkGetPhysicalDeviceSurfaceFormats2KHR(device, &surface_info, &format_count, out_info.formats.data()));

    u32 present_mode_count = 0;

    RT_VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr));

    out_info.present_modes.resize(present_mode_count);
    RT_VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, out_info.present_modes.data()));

    return true;
}

i32 VulkanDeviceCombination::find_memory_index(u32 type_filter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties2& memory_properties = this->memory_properties;
    for (u32 i = 0; i < memory_properties.memoryProperties.memoryTypeCount; ++i) {
        if ((type_filter & (1 << i)) && (memory_properties.memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    RT_LOG_WARN_PLATFORM("Failed to find suitable memory type.");
    return -1;
}

}
