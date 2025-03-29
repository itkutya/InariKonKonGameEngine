#pragma once

#include <unordered_map>

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

#include "InariKonKon/Graphics/Renderer/Vulkan/Buffer/VertexBuffer.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/Buffer/IndexBuffer.hpp"

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

        template<class VertexType, class IndiciesType>
        void draw(const Model<VertexType, IndiciesType>& model) noexcept;
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

        struct RenderBuffer
        {
            std::shared_ptr<Buffer> vertexBuffer = nullptr;
            std::shared_ptr<Buffer> indexBuffer = nullptr;
        };

        std::unordered_map<std::uint32_t, std::vector<RenderBuffer>> m_objects;

        void resizeToWindow() noexcept;
    };

    template <class VertexType, class IndiciesType>
    void Vulkan::draw(const Model<VertexType, IndiciesType>& model) noexcept
    {
        static bool once = true;
        if (once)
        {
            const auto& vertecies = model.getVertecies();
            const auto& indicies = model.getIndicies();

            const RenderBuffer& buffer =
                {
                    .vertexBuffer = std::make_shared<VertexBuffer<VertexType>>(this->m_logicalDevice, this->m_physicalDevice, vertecies),
                    .indexBuffer = nullptr
                };

            this->m_graphicsPipelines.emplace_back(this->m_logicalDevice, this->m_renderpass,
                model.getFragmentShader(), model.getVertexShader(), model.getVertexInfo());

            this->m_objects.emplace(std::make_pair(0, std::vector<RenderBuffer>{}));
            this->m_objects.at(0).emplace_back(buffer);

            once = false;
        }
    }
}