#pragma once

#include "InariKonKon/Renderer/Vulkan/Instance.hpp"
#include "InariKonKon/Renderer/Vulkan/Surface.hpp"
#include "InariKonKon/Renderer/Vulkan/PhysicalDevice.hpp"
#include "InariKonKon/Renderer/Vulkan/LogicalDevice.hpp"
#include "InariKonKon/Renderer/Vulkan/Swapchain.hpp"
#include "InariKonKon/Renderer/Vulkan/Renderpass.hpp"
#include "InariKonKon/Renderer/Vulkan/GraphicsPipeline.hpp"
#include "InariKonKon/Renderer/Vulkan/Framebuffer.hpp"
#include "InariKonKon/Renderer/Vulkan/CommandPool.hpp"
#include "InariKonKon/Renderer/Vulkan/CommandBuffer.hpp"
//TODO:
//Place into folders the rest of it...
#include "InariKonKon/Renderer/Vulkan/Buffer/VertexBuffer.hpp"
#include "InariKonKon/Renderer/Vulkan/Buffer/IndexBuffer.hpp"

#include "InariKonKon/Renderer/RendererBase.hpp"

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

        void beginRender(const Viewport& viewport, const u32Rect& scissor) override;
        void endRender() override;

        const fRect getSurface() const noexcept override;
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

        //TODO:
        //Model drawing...
        //When drawn first time -> create it's stuff...
        //Then reuse it... -> then delete it
        //Prob use std::weak_ptr...
        std::vector<VertexBuffer> m_vertexBuffers;
        std::vector<IndexBuffer> m_indexBuffers;

        void resizeToWindow() noexcept;
    };
}