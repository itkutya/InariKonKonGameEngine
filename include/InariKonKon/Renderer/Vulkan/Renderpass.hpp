#pragma once

#include "InariKonKon/Renderer/Vulkan/Swapchain.hpp"

namespace ikk
{
    class Framebuffer;

    class Renderpass : public VkType<VkRenderPass>
    {
    public:
        Renderpass(LogicalDevice& logicalDevice, Swapchain& swapchain);

        ~Renderpass() noexcept;

        Renderpass(const Renderpass&) noexcept = default;
        Renderpass(Renderpass&&) noexcept = default;

        Renderpass& operator=(const Renderpass&) noexcept = default;
        Renderpass& operator=(Renderpass&&) noexcept = default;
    protected:
        void begin(VkCommandBuffer& commandBuffer, Swapchain& swapchain, Framebuffer& framebuffer) noexcept;
        void end(VkCommandBuffer& commandBuffer) noexcept;
    private:
        LogicalDevice* m_logicalDevice;

        friend class CommandBuffer;
    };
}