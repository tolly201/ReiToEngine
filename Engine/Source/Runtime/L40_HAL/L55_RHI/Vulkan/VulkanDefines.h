#ifndef RHI_VULKAN_DEFINES_H
#define RHI_VULKAN_DEFINES_H
#include "vulkan/vulkan.h"
#include "L20_Platform/Include.h"
#include "../RendererTypes.h"
namespace ReiToEngine
{
struct VulkanCommandBuffer;
struct VulkanShaderSet;

enum class VulkanQueueFamilyIndicesType : u8{
    GRAPHICS = 0,
    PRESENT = 1,
    COMPUTE = 2,
    TRANSFER = 3,
    MAX = 4,
};

struct VulkanBuffer{
    u64 total_size;
    VkBuffer handle;
    VkBufferUsageFlagBits usage;
    b8 is_locked;
    VkDeviceMemory memory;
    i32 memory_index;
    u32 memory_property_flags;
};

struct VulkanMesh
{
    VulkanBuffer vertex_buffer;
    VulkanBuffer index_buffer;
    u32 vertex_count = 0;
    u32 index_count  = 0;
    VkIndexType index_type = VK_INDEX_TYPE_UINT32;
};

struct VulkanPhysicalDeviceRequirements{
    Map<VulkanQueueFamilyIndicesType, b8> queue_families;

    List<const char*> required_extensions;
    b8 sample_anisotropy;
    b8 discrete_gpu;
};

struct VulkanDeviceCombination{
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties2 device_properties;
    VkPhysicalDeviceFeatures2 device_features;
    VkPhysicalDeviceMemoryProperties2 memory_properties;
    List<VkQueueFamilyProperties2> queue_families;
    List<VkExtensionProperties> supported_extensions;

    Map<VulkanQueueFamilyIndicesType, List<i32>> available_queue_family_indices;

    Map<VulkanQueueFamilyIndicesType, List<i32>> dedicated_queue_family_indices;

    Map<VulkanQueueFamilyIndicesType, VkQueue> queues;
    Map<VulkanQueueFamilyIndicesType, List<i32>> inuse_queue_family_indices;
    VkFormat depth_format;

    // mark logical device is initialized
    // recover logical device when all swapchains using this device are destroyed
    VkDevice logical_device;
    Map<VulkanQueueFamilyIndicesType, VkCommandPool> command_pools;
    Map<VulkanQueueFamilyIndicesType, List<VulkanCommandBuffer>> command_buffers;

    b8 is_inused;
    Set<i32> using_swapchains;
    i32 find_memory_index(u32 type_filter, VkMemoryPropertyFlags properties);

    List<VulkanShaderSet> shader_sets;

    VulkanBuffer vertex_buffer;
    VulkanBuffer index_buffer;
    u64 geometry_vertex_offset;
    u64 geometry_index_offset;

    VkDescriptorSetLayout set0_global_layout;   // projection/view 等
    VkDescriptorSetLayout set1_material_layout; // 材质参数 + 纹理
    VkDescriptorSetLayout set2_object_layout;   // 模型矩阵/实例数据（若使用）
    VkDescriptorPool device_descriptor_pool;           // 一个或少量池，容量按需要预留
    VkPipelineCache pipeline_cache;             // 可选，加速管线创建
};

struct VulkanSwapChainSupportInfo
{
    VkSurfaceCapabilities2KHR capabilities;
    List<VkSurfaceFormat2KHR> formats;
    List<VkPresentModeKHR> present_modes;
};

struct VulkanImage{
    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    VkFormat format;
    u32 width;
    u32 height;
    u32 depth;
    u32 mip_levels;
    u32 array_layers;
    VkSampleCountFlagBits samples;
    VkSharingMode sharing_mode;
};


enum class VulkanRenderPassState : u8{
    INITIAL,
    READY,
    RECORDING,
    RECORDED,
    SUBMITTED,
    NOT_ALLOCATED,
    MAX
};

struct VulkanRenderPass{
    VkRenderPass handle;
    f32 x, y, width, height;
    f32 r, g, b, a;

    f32 depth;
    u32 stencil;
    VulkanRenderPassState state;
};

enum class VulkanCommandBufferState : u8{
    INITIAL,
    READY,
    RECORDING,
    RECORDED,
    RENDERING,
    RENDERED,
    SUBMITTED,
    NOT_ALLOCATED,
    MAX
};

struct VulkanCommandBuffer{
    VkCommandBuffer handle;
    VulkanCommandBufferState state;
};

struct VulkanFrameBuffer{
    VkFramebuffer handle;
    u32 attachment_count;
    VkImageView* p_attachments;
    VulkanRenderPass* p_render_pass;
};

struct VulkanFence{
    VkFence handle;
    b8 signaled;
};

struct VulkanShaderStage{
    VkShaderModuleCreateInfo module_create_info;
    VkShaderModule handle;
    VkPipelineShaderStageCreateInfo stage_create_info;
    b8 is_valid;
};

struct VulkanPipeline{
    VkPipelineLayout layout;
    VkPipeline handle;
};

enum class ShaderStageType : u8{
    VERTEX = 0,
    FRAGMENT = 1,
    COMPUTE = 2,
    MAX = 3,
};

struct VulkanPerFrame {
    VulkanCommandBuffer cmd;
    VulkanFence fence;
    VkSemaphore image_available, render_finished;

    // 每帧全局 UBO + 对应的 DescriptorSet（set=0）
    VulkanBuffer global_ubo_buffer;     // VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
    VkDescriptorSet descriptionSet; // 从 swapchain.descriptor_pool 分配
};

struct VulkanShaderSet{
    VulkanShaderStage shader_stages[static_cast<u8>(ShaderStageType::MAX)];
    VulkanPipeline pipeline;
    VulkanBuffer global_uniform_buffer;
    List<VkDescriptorSet> description_set; // 从 swapchain.descriptor_pool 分配
    global_uniform_object global_ubo;

    VulkanPerFrame tempframe;
};

struct VulkanSwapchainContext{
    VkSurfaceKHR surface;
    VulkanSwapChainSupportInfo swapchain_info;
    VulkanDeviceCombination* device_combination;
    Map<VulkanQueueFamilyIndicesType, i32> queue_family_indices;
    u32 width;
    u32 height;
    void* p_window;

    VkSurfaceFormat2KHR selected_surface_format;
    u8 max_frames_in_flight;

    List<VulkanPerFrame> per_frames;

    VkSwapchainKHR swapchain;
    u32 image_count;
    List<VulkanImage> images;
    List<VulkanFrameBuffer> framebuffers;
    u32 current_image_index;
    u32 current_frame;
    b8 recreating_swapchain;

    List<VkSemaphore> image_available_semaphores;
    List<VkSemaphore> render_finished_semaphores;

    u32 in_flight_fence_count;
    List<VulkanFence> in_flight_fences;
    List<VulkanFence*> images_in_flight;

    VulkanImage depth_image;
    VulkanPhysicalDeviceRequirements requirements;

    VulkanRenderPass render_pass;
    VkDescriptorPool swapchain_per_frame_descriptor_pool;
    i32 index;
    u32 frame_size_generation;
    u32 last_frame_size_generation;
};

struct VulkanContextRef{
    VkInstance instance = VK_NULL_HANDLE;
    VkAllocationCallbacks* allocator = nullptr;
    VulkanDeviceCombination* device_combination = nullptr;
    VulkanSwapchainContext* swapchain_context = nullptr;
};

RT_FORCEINLINE b8 RT_VK_CHECK(VkResult expr) {
    RT_ASSERT_PLATFORM(expr == VK_SUCCESS);
    return expr == VK_SUCCESS;
}

}

#endif
