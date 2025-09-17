#include "VulkanDefines.h"
namespace ReiToEngine
{
void vulkan_frame_buffer_create(VulkanContextRef context, VulkanSwapchainContext& swapchain_context,
VulkanRenderPass& render_pass, u32 width, u32 height,
u32 attachment_count, VkImageView* p_attachments,
VulkanFrameBuffer& out_framebuffer)
{
    out_framebuffer.attachment_count = attachment_count;
    out_framebuffer.p_attachments = (VkImageView*)GetMemoryManager().Allocate(sizeof(VkImageView) * out_framebuffer.attachment_count, 8, RT_MEMORY_TAG::RENDERER);
    for (u32 i = 0; i < out_framebuffer.attachment_count; ++i)
    {
        out_framebuffer.p_attachments[i] = p_attachments[i];
    }
    out_framebuffer.p_render_pass = &render_pass;

    VkFramebufferCreateInfo fbci{};
    fbci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbci.renderPass = render_pass.handle;
    fbci.attachmentCount = out_framebuffer.attachment_count;
    fbci.pAttachments = out_framebuffer.p_attachments;
    fbci.width = width;
    fbci.height = height;
    fbci.layers = 1;
    fbci.flags = 0;
    fbci.pNext = nullptr;

    RT_VK_CHECK(vkCreateFramebuffer(swapchain_context.device_combination->logical_device, &fbci, context.allocator, &out_framebuffer.handle));
}

void vulkan_frame_buffer_destroy(VulkanContextRef context, VulkanSwapchainContext& swapchain_context, VulkanFrameBuffer& framebuffer)
{
    if (framebuffer.p_attachments)
    {
        GetMemoryManager().Free(framebuffer.p_attachments, framebuffer.attachment_count, RT_MEMORY_TAG::RENDERER);
        framebuffer.p_attachments = nullptr;
        framebuffer.attachment_count = 0;
    }

    if (framebuffer.handle != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(swapchain_context.device_combination->logical_device, framebuffer.handle, context.allocator);
        framebuffer.handle = VK_NULL_HANDLE;
    }
    framebuffer.p_render_pass = nullptr;
    framebuffer.attachment_count = 0;
}

}
