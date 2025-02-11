#include "InariKonKon/Renderer/Vulkan/Renderpass.hpp"

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"
#include "InariKonKon/Renderer/Vulkan/Framebuffer.hpp"

namespace ikk
{
    Renderpass::Renderpass(LogicalDevice& logicalDevice, Swapchain& swapchain)
    try : m_logicalDevice(&logicalDevice)
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapchain.getFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VK_CHECK(vkCreateRenderPass(this->m_logicalDevice->getUnderlyingVkType(), &renderPassInfo, nullptr, &this->m_type));
    }
    catch(const std::exception& e)
    {
        std::print("{}", e.what());
    }

    Renderpass::~Renderpass() noexcept
    {
        vkDestroyRenderPass(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, nullptr);
    }

    void Renderpass::begin(VkCommandBuffer& commandBuffer, Swapchain& swapchain, Framebuffer& framebuffer) noexcept
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = this->m_type;
        renderPassInfo.framebuffer = framebuffer.getUnderlyingVkType();
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapchain.getExtent();

        VkClearValue clearColor = { .color = { .float32 = { 0.0f, 0.0f, 0.5f, 1.0f } } };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void Renderpass::end(VkCommandBuffer& commandBuffer) noexcept
    {
        vkCmdEndRenderPass(commandBuffer);
    }
}