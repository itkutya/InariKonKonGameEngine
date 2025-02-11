#pragma once

#include "InariKonKon/Renderer/Vulkan/GraphicsPipeline.hpp"
#include "InariKonKon/Renderer/Vulkan/CommandPool.hpp"

#include "InariKonKon/Window/Viewport.hpp"

namespace ikk
{
    class CommandBuffer : public VkType<VkCommandBuffer>
    {
    public:
        CommandBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool);

        ~CommandBuffer() noexcept;

        CommandBuffer(const CommandBuffer&) noexcept = default;
        CommandBuffer(CommandBuffer&&) noexcept = default;

        CommandBuffer& operator=(const CommandBuffer&) noexcept = default;
        CommandBuffer& operator=(CommandBuffer&&) noexcept = default;

        void startRecording(const VkCommandBufferUsageFlagBits flags = static_cast<VkCommandBufferUsageFlagBits>(0), const bool resetResources = false);
        void beginRenderpass(Renderpass& renderpass, Swapchain& swapchain, Framebuffer& framebuffer) noexcept;

        void bindGraphicspipeline(GraphicsPipeline& graphicspipeline, const VkPipelineBindPoint pipelineBindPoint) noexcept;
        void setViewport(const Viewport& viewport) noexcept;
        void setScissor(const u32Rect& scissor) noexcept;
        
        void endRecording();
        void endRenderpass(Renderpass& renderpass) noexcept;
    private:
        LogicalDevice* m_logicalDevice;
        CommandPool* m_commandPool;
    };
}