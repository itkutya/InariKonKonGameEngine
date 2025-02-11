#pragma once

#include "InariKonKon/Renderer/Vulkan/Renderpass.hpp"

namespace ikk
{
    class Framebuffer : public VkType<VkFramebuffer>
    {
    public:
        Framebuffer(LogicalDevice& logicalDevice, Swapchain& swapchain, Renderpass& renderpass, const std::size_t index);

        ~Framebuffer() noexcept;

        Framebuffer(const Framebuffer&) noexcept = default;
        Framebuffer(Framebuffer&&) noexcept = default;

        Framebuffer& operator=(const Framebuffer&) noexcept = default;
        Framebuffer& operator=(Framebuffer&&) noexcept = default;

        void create(Swapchain& swapchain, Renderpass& renderpass, const std::size_t index);
        void destroy() noexcept;
    private:
        LogicalDevice* m_logicalDevice;
    };
}