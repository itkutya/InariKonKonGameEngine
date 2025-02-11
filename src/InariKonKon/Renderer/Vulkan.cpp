#include "InariKonKon/Renderer/Vulkan.hpp"

#define GLFW_INCLUDE_VULKAN
#include "glfw/glfw3.h"

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

#define MAX_FRAMES_IN_FLIGHT 2

namespace ikk
{
    Vulkan::Vulkan(std::u8string_view title, GLFWwindow* window, const std::uint32_t width, const std::uint32_t height) noexcept
        :   m_window(window),
            m_instance(title),
            m_surface(m_instance, m_window),
            m_physicalDevice(m_instance),
            m_logicalDevice(m_instance, m_physicalDevice, m_surface),
            m_swapchain(width, height, m_surface, m_physicalDevice, m_logicalDevice, MAX_FRAMES_IN_FLIGHT),
            m_renderpass(m_logicalDevice, m_swapchain),
            m_commandPool(m_logicalDevice)
    {
        this->m_framebuffers.reserve(this->m_swapchain.getImageViews().size());
        for (std::size_t i = 0; i < this->m_swapchain.getImageViews().size(); ++i)
            this->m_framebuffers.emplace_back(this->m_logicalDevice, this->m_swapchain, this->m_renderpass, i);
        
        this->m_commandBuffers.reserve(MAX_FRAMES_IN_FLIGHT);
        for (std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            this->m_commandBuffers.emplace_back(this->m_logicalDevice, this->m_commandPool);
        
        //TODO:
        //temp...
        Shader vertex{ Filesystem{ "shaders/shader.vert" }, Shader::VERTEX };
        Shader fragment { Filesystem{ "shaders/shader.frag" }, Shader::FRAGMENT };
        this->m_graphicsPipelines.emplace_back(this->m_logicalDevice, this->m_renderpass, vertex, fragment);

        const std::vector<Vertex> verticies = std::vector<Vertex>{{
                                                                    { { -0.5f, -0.5f, 0.f }, { 1.0f, 0.0f, 0.0f } },
                                                                    { {  0.5f, -0.5f, 0.f }, { 0.0f, 1.0f, 0.0f } },
                                                                    { {  0.5f,  0.5f, 0.f }, { 0.0f, 0.0f, 1.0f } },
                                                                    { { -0.5f,  0.5f, 0.f }, { 1.0f, 1.0f, 1.0f } }
                                                                  }};
        const std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
        
        this->m_vertexBuffers.emplace_back(this->m_logicalDevice, this->m_physicalDevice, verticies, this->m_commandPool);
        this->m_indexBuffers.emplace_back(this->m_logicalDevice, this->m_physicalDevice, indices, this->m_commandPool);
    }

    void Vulkan::onResize(const std::uint32_t width, const std::uint32_t height) noexcept
    {
        this->m_windowResized = true;
    }

    void Vulkan::beginRender(const Viewport& viewport, const u32Rect& scissor)
    {
        const VkResult result = this->m_swapchain.acquireNextImage(&this->m_imageIndex, UINT64_MAX, this->m_currentFrame);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            this->resizeToWindow();
            this->m_renderStarted = false;
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("Failed to acquire swap chain image!");

        this->m_renderStarted = true;
        
        CommandBuffer& commandBuffer = this->m_commandBuffers.at(this->m_currentFrame);
        
        commandBuffer.startRecording();
        commandBuffer.beginRenderpass(this->m_renderpass, this->m_swapchain, this->m_framebuffers.at(this->m_imageIndex));
        
        for (GraphicsPipeline& graphicsPipeline : this->m_graphicsPipelines)
        {
            commandBuffer.bindGraphicspipeline(graphicsPipeline, VK_PIPELINE_BIND_POINT_GRAPHICS);
            commandBuffer.setViewport(viewport);
            commandBuffer.setScissor(scissor);
            //TODO:
            //Draw models...
            for (const Buffer& buffer : this->m_vertexBuffers)
            {
                VkBuffer vertexBuffers[] = { buffer.getUnderlyingVkType() };
                VkDeviceSize offsets[] = { 0 };
                vkCmdBindVertexBuffers(commandBuffer.getUnderlyingVkType(), 0, 1, vertexBuffers, offsets);
                
                vkCmdBindIndexBuffer(commandBuffer.getUnderlyingVkType(), this->m_indexBuffers.at(0).getUnderlyingVkType(), 0, VK_INDEX_TYPE_UINT32);
                
                vkCmdDrawIndexed(commandBuffer.getUnderlyingVkType(), 6, 1, 0, 0, 0);
                //vkCmdDraw(commandBuffer.getUnderlyingVkType(), 3, 1, 0, 0);
            }
        }
    }

    void Vulkan::endRender()
    {
        if (this->m_renderStarted == false)
            return;

        CommandBuffer& commandBuffer = this->m_commandBuffers.at(this->m_currentFrame);

        commandBuffer.endRenderpass(this->m_renderpass);
        commandBuffer.endRecording();   

        VK_CHECK(this->m_swapchain.submit(commandBuffer, this->m_currentFrame));

        const VkResult result = this->m_swapchain.present(&this->m_imageIndex, this->m_currentFrame);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || this->m_windowResized == true)
            this->resizeToWindow();
        else if (result != VK_SUCCESS)
            throw std::runtime_error("Failed to present swap chain image!");

        this->m_currentFrame = (this->m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    //TODO:
    //Make this better...
    const fRect Vulkan::getSurface() const noexcept
    {
        return fRect{ 0.f, 0.f, static_cast<float>(this->m_swapchain.getExtent().width), static_cast<float>(this->m_swapchain.getExtent().height) };
    }

    void Vulkan::resizeToWindow() noexcept
    {
        int width = 0, height = 0;
        glfwGetWindowSize(this->m_window, &width, &height);
        
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(this->m_window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(this->m_logicalDevice.getUnderlyingVkType());

        this->m_windowResized = false;

        this->m_swapchain.destroy();
        for (auto& framebuffer : this->m_framebuffers)
            framebuffer.destroy();

        this->m_swapchain.create(static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height), this->m_physicalDevice);
        for (std::size_t i = 0; i < this->m_swapchain.getImageViews().size(); ++i)
            this->m_framebuffers.at(i).create(this->m_swapchain, this->m_renderpass, i);
    }
}