#include "InariKonKon/Graphics/Renderer/Vulkan.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

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
    }

    void Vulkan::onResize([[maybe_unused]] const std::uint32_t width, [[maybe_unused]] const std::uint32_t height) noexcept
    {
        this->m_windowResized = true;
    }

    void Vulkan::beginRender()
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
            
            commandBuffer.setViewport();
            commandBuffer.setScissor();
            
            const auto range = this->m_objects.equal_range(graphicsPipeline);
            for (auto it = range.first; it != range.second; ++it)
            {
                //TODO:
                //Draw...
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

    void Vulkan::renderObject(const Model<int, int>& model) noexcept
    {
        //this->m_graphicsPipelines.emplace_back(this->m_logicalDevice, this->m_renderpass, model.vertex, model.fragment, model.vertex);
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

        this->m_swapchain.create(U32(width), U32(height), this->m_physicalDevice);
        for (std::size_t i = 0; i < this->m_swapchain.getImageViews().size(); ++i)
            this->m_framebuffers.at(i).create(this->m_swapchain, this->m_renderpass, i);
    }
}