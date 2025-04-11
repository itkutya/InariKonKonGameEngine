#ifndef IKK_VULKAN_HPP
#define IKK_VULKAN_HPP

#include "InariKonKon/Graphics/Renderer/Vulkan/Instance.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/Surface.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/PhysicalDevice.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/LogicalDevice.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/Swapchain.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/Renderpass.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/GraphicsPipeline.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/Framebuffer.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/CommandPool.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/CommandBuffer.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/Buffer/Buffer.hpp"

#include "InariKonKon/Graphics/Renderer/RendererBase.hpp"

namespace ikk
{
    class Vulkan final : public RendererBase
    {
    public:
        Vulkan(std::u8string_view title, GLFWwindow* window, const std::uint32_t width, const std::uint32_t height) noexcept;

        ~Vulkan() noexcept = default;

        Vulkan(const Vulkan&) noexcept = default;
        Vulkan(Vulkan&&) noexcept = default;

        Vulkan& operator=(const Vulkan&) noexcept = default;
        Vulkan& operator=(Vulkan&&) noexcept = default;
        
        void onResize([[maybe_unused]] const std::uint32_t width, [[maybe_unused]] const std::uint32_t height) noexcept override;

        void beginRender() override;
        void endRender() override;
    private:
        GLFWwindow* m_window;

        Instance m_instance;
        Surface m_surface;
        PhysicalDevice m_physicalDevice;
        LogicalDevice m_logicalDevice;
        Swapchain m_swapchain;
        Renderpass m_renderpass;
        std::vector<GraphicsPipeline> m_graphicsPipelines;
        std::vector<Framebuffer> m_framebuffers;
        CommandPool m_commandPool;
        std::vector<CommandBuffer> m_commandBuffers;

        std::uint32_t m_currentFrame = 0;
        std::uint32_t m_imageIndex = 0;
        
        bool m_windowResized = false;
        bool m_renderStarted = false;

        struct RenderBuffers
        {
            Buffer vertexBuffer;
            Buffer indexBuffer;
        };
        std::vector<RenderBuffers> m_renderBuffers;

        void resizeToWindow() noexcept;

        void draw(const RenderObject& renderObj) noexcept override;
    };
}

#endif