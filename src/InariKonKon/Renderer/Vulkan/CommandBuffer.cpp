#include "InariKonKon/Renderer/Vulkan/CommandBuffer.hpp"

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    CommandBuffer::CommandBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool)
    try : m_logicalDevice(&logicalDevice), m_commandPool(&commandPool)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = this->m_commandPool->getUnderlyingVkType();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1U;

        VK_CHECK(vkAllocateCommandBuffers(this->m_logicalDevice->getUnderlyingVkType(), &allocInfo, &this->m_type));
    }
    catch(const std::exception& e)
    {
        std::print("{}\n", e.what());
    }

    CommandBuffer::~CommandBuffer() noexcept
    {
        vkDeviceWaitIdle(this->m_logicalDevice->getUnderlyingVkType());
        vkFreeCommandBuffers(this->m_logicalDevice->getUnderlyingVkType(), this->m_commandPool->getUnderlyingVkType(), 1U, &this->m_type);
    }

    void CommandBuffer::startRecording(const VkCommandBufferUsageFlagBits flags, const bool resetResources)
    {
        VK_CHECK(vkResetCommandBuffer(m_type,
            resetResources ? VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : static_cast<VkCommandBufferResetFlagBits>(0)));

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;

        VK_CHECK(vkBeginCommandBuffer(this->m_type, &beginInfo));
    }

    void CommandBuffer::beginRenderpass(Renderpass& renderpass, Swapchain& swapchain, Framebuffer& framebuffer) noexcept
    {
        renderpass.begin(this->m_type, swapchain, framebuffer);
    }

    void CommandBuffer::bindGraphicspipeline(GraphicsPipeline& graphicspipeline, const VkPipelineBindPoint pipelineBindPoint) noexcept
    {
        graphicspipeline.bind(this->m_type, pipelineBindPoint);
    }

    void CommandBuffer::setViewport(const Viewport& viewport) noexcept
    {
        const auto& rect = viewport.getRect();
        VkViewport vp{};
        vp.x = rect.x;
        vp.y = rect.y;
        vp.width = rect.width;
        vp.height = rect.height;
        vp.minDepth = viewport.getMinDepth();
        vp.maxDepth = viewport.getMaxDepth();
        vkCmdSetViewport(this->m_type, 0, 1, &vp);
    }

    void CommandBuffer::setScissor(const u32Rect& scissor) noexcept
    {
        VkRect2D sr{};
        sr.offset = { static_cast<std::int32_t>(scissor.x), static_cast<std::int32_t>(scissor.y) };
        sr.extent = { scissor.width, scissor.height };
        vkCmdSetScissor(this->m_type, 0, 1, &sr);
    }

    void CommandBuffer::endRecording()
    {
        VK_CHECK(vkEndCommandBuffer(this->m_type));
    }

    void CommandBuffer::endRenderpass(Renderpass& renderpass) noexcept
    {
        renderpass.end(this->m_type);
    }
}