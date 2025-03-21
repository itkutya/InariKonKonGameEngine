#include "InariKonKon/Graphics/Renderer/Vulkan/CommandBuffer.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    CommandBuffer::CommandBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool) noexcept
        : m_logicalDevice(&logicalDevice), m_commandPool(&commandPool)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = this->m_commandPool->getUnderlyingVkType();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1U;

        VK_CHECK(vkAllocateCommandBuffers(this->m_logicalDevice->getUnderlyingVkType(), &allocInfo, &this->m_type));

        DEBUG_LOG("Vulkan command buffer created.", Log::INFO, Log::ALL);
    }

    CommandBuffer::~CommandBuffer() noexcept
    {
        vkDeviceWaitIdle(this->m_logicalDevice->getUnderlyingVkType());
        vkFreeCommandBuffers(this->m_logicalDevice->getUnderlyingVkType(), this->m_commandPool->getUnderlyingVkType(), 1U, &this->m_type);

        DEBUG_LOG("Vulkan command buffer destroyed.", Log::INFO, Log::ALL);
    }

    void CommandBuffer::startRecording(const VkCommandBufferUsageFlags flags, const bool resetResources) noexcept
    {
        VK_CHECK(vkResetCommandBuffer(this->m_type, resetResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0));

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

    void CommandBuffer::setViewport() noexcept
    {
        //TODO:
        //FIX
        VkViewport viewport{};
        viewport.x = 0.f;
        viewport.y = 0.f;
        viewport.width = 800.f;
        viewport.height = 600.f;
        viewport.minDepth = 0.f;
        viewport.maxDepth = 1.f;
        vkCmdSetViewport(this->m_type, 0, 1, &viewport);
    }

    void CommandBuffer::setScissor() noexcept
    {
        //TODO
        //FIX
        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = { 800, 600 };
        vkCmdSetScissor(this->m_type, 0, 1, &scissor);
    }

    void CommandBuffer::endRecording() noexcept
    {
        VK_CHECK(vkEndCommandBuffer(this->m_type));
    }

    void CommandBuffer::endRenderpass(Renderpass& renderpass) noexcept
    {
        renderpass.end(this->m_type);
    }
}