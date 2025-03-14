#pragma once

#include "InariKonKon/Graphics/Renderer/Vulkan/Renderpass.hpp"

namespace ikk
{
    class Framebuffer final : public VkType<VkFramebuffer>
    {
    public:
        Framebuffer(LogicalDevice& logicalDevice, Swapchain& swapchain, Renderpass& renderpass, const std::size_t index) noexcept;

        ~Framebuffer() noexcept;

        Framebuffer(const Framebuffer&) noexcept = default;
        Framebuffer(Framebuffer&&) noexcept = default;

        Framebuffer& operator=(const Framebuffer&) noexcept = default;
        Framebuffer& operator=(Framebuffer&&) noexcept = default;

        void create(Swapchain& swapchain, Renderpass& renderpass, const std::size_t index) noexcept;
        void destroy() noexcept;
    private:
        LogicalDevice* m_logicalDevice = nullptr;
    };
}