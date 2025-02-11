#include "InariKonKon/Renderer/Vulkan/Framebuffer.hpp"

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    Framebuffer::Framebuffer(LogicalDevice& logicalDevice, Swapchain& swapchain, Renderpass& renderpass, const std::size_t index)
    try : m_logicalDevice(&logicalDevice)
    {
        this->create(swapchain, renderpass, index);
    }
    catch(const std::exception& e)
    {
        std::print("{}\n", e.what());
    }

    Framebuffer::~Framebuffer() noexcept
    {
        this->destroy();
    }

    void Framebuffer::create(Swapchain &swapchain, Renderpass &renderpass, const std::size_t index)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderpass.getUnderlyingVkType();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &swapchain.getImageViews().at(index);
        framebufferInfo.width = swapchain.getExtent().width;
        framebufferInfo.height = swapchain.getExtent().height;
        framebufferInfo.layers = 1;

        VK_CHECK(vkCreateFramebuffer(this->m_logicalDevice->getUnderlyingVkType(), &framebufferInfo, nullptr, &this->m_type));
    }

    void Framebuffer::destroy() noexcept
    {
        vkDestroyFramebuffer(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, nullptr);
    }
}