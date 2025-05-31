#ifndef IKK_COMMAND_BUFFER_HPP
#define IKK_COMMAND_BUFFER_HPP

#include "InariKonKon/Graphics/Renderer/Vulkan/GraphicsPipeline.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/CommandPool.hpp"

namespace ikk
{
    class CommandBuffer final : public VkType<VkCommandBuffer>
    {
    public:
        CommandBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool) noexcept;

        ~CommandBuffer() noexcept;

        CommandBuffer(const CommandBuffer&) noexcept = default;
        CommandBuffer(CommandBuffer&&) noexcept = default;

        CommandBuffer& operator=(const CommandBuffer&) noexcept = default;
        CommandBuffer& operator=(CommandBuffer&&) noexcept = default;

        void startRecording(const VkCommandBufferUsageFlags flags = 0, const bool resetResources = false) noexcept;
        void beginRenderpass(Renderpass& renderpass, Swapchain& swapchain, Framebuffer& framebuffer) noexcept;

        void bindGraphicspipeline(GraphicsPipeline& graphicspipeline, const VkPipelineBindPoint pipelineBindPoint) noexcept;
        
        void setViewport(GLFWwindow* window) noexcept;
        void setScissor(GLFWwindow* window) noexcept;
        
        void endRecording() noexcept;
        void endRenderpass(Renderpass& renderpass) noexcept;
    private:
        LogicalDevice* m_logicalDevice = nullptr;
        CommandPool* m_commandPool = nullptr;
    };
}

#endif