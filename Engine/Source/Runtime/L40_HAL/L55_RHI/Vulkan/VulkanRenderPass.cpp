#include "VulkanRenderPass.h"
namespace ReiToEngine
{
void vulkan_renderpass_create(VulkanContextRef context, VulkanSwapchainContext& swapchain_context, VulkanRenderPass& out_render_pass)
{

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    u32 attachment_description_count = 2;
    List<VkAttachmentDescription> attachment_descriptions;
    attachment_descriptions.resize(attachment_description_count);

    VkAttachmentDescription& color_attachment = attachment_descriptions[0];
    color_attachment.format = swapchain_context.selected_surface_format.surfaceFormat.format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    color_attachment.flags = 0;

    VkAttachmentReference color_attachment_ref{};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkAttachmentDescription& depth_attachment = attachment_descriptions[1];
    depth_attachment.format = swapchain_context.depth_image.format;
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_attachment_ref{};

    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    subpass.pDepthStencilAttachment = &depth_attachment_ref;

    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.pResolveAttachments = nullptr;

    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dependencyFlags = 0;

    VkRenderPassCreateInfo rpci{};
    rpci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpci.attachmentCount = attachment_description_count;
    rpci.pAttachments = attachment_descriptions.data();
    rpci.subpassCount = 1;
    rpci.pSubpasses = &subpass;
    rpci.dependencyCount = 1;
    rpci.pDependencies = &dependency;
    rpci.pNext = nullptr;
    rpci.flags = 0;

    RT_VK_CHECK(vkCreateRenderPass(swapchain_context.device_combination->logical_device, &rpci, context.allocator, &out_render_pass.render_pass));
}

void vulkan_renderpass_destroy(VulkanContextRef context, VulkanSwapchainContext& swapchain_context, VulkanRenderPass& out_render_pass)
{
    if (out_render_pass.render_pass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(swapchain_context.device_combination->logical_device, out_render_pass.render_pass, context.allocator);
    }
}

void vulkan_renderpass_begin(VulkanContextRef context, VulkanSwapchainContext& swapchain_context,VulkanRenderPass& render_pass, VulkanCommandBuffer& command_buffer, VkFramebuffer framebuffer)
{
    VkRenderPassBeginInfo rpbi{};
    rpbi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpbi.renderPass = render_pass.render_pass;
    rpbi.framebuffer = framebuffer;
    rpbi.renderArea.offset.x = render_pass.x;
    rpbi.renderArea.offset.y = render_pass.y;
    rpbi.renderArea.extent.width = render_pass.width;
    rpbi.renderArea.extent.height = render_pass.height;

    List<VkClearValue> clear_values;
    clear_values.resize(2);
    clear_values[0].color.float32[0] = render_pass.r;
    clear_values[0].color.float32[1] = render_pass.g;
    clear_values[0].color.float32[2] = render_pass.b;
    clear_values[0].color.float32[3] = render_pass.a;
    clear_values[1].depthStencil.depth = render_pass.depth;
    clear_values[1].depthStencil.stencil = render_pass.stencil;

    rpbi.clearValueCount = clear_values.size();
    rpbi.pClearValues = clear_values.data();

    vkCmdBeginRenderPass(command_buffer.command_buffer, &rpbi, VK_SUBPASS_CONTENTS_INLINE);
    command_buffer.state = VulkanCommandBufferState::RENDERING;
}

void vulkan_renderpass_end(VulkanContextRef context, VulkanSwapchainContext& swapchain_context,VulkanRenderPass& render_pass, VulkanCommandBuffer& command_buffer, VkFramebuffer framebuffer)
{
    vkCmdEndRenderPass(command_buffer.command_buffer);
    command_buffer.state = VulkanCommandBufferState::RECORDING;

}
}
