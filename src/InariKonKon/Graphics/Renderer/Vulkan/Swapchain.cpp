#include "InariKonKon/Graphics/Renderer/Vulkan/Swapchain.hpp"

#include <limits>
#include <algorithm>

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan/CommandBuffer.hpp"

namespace ikk
{
    Swapchain::Swapchain(const std::uint32_t width, const std::uint32_t height, Surface& surface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, const std::size_t maxFramesInFlight) noexcept
        : m_surface(&surface), m_logicalDevice(&logicalDevice)
    {
        this->create(width, height, physicalDevice);

        this->m_frameSyncObjects.reserve(maxFramesInFlight);
        for (std::size_t i = 0; i < maxFramesInFlight; ++i)
            this->m_frameSyncObjects.emplace_back(*this->m_logicalDevice);
    }
    
    Swapchain::~Swapchain() noexcept
    {
        this->destroy();
    }

    void Swapchain::create(const std::uint32_t width, const std::uint32_t height, PhysicalDevice& physicalDevice) noexcept
    {   
        this->m_surface->chooseSurfaceFormat(physicalDevice);
        this->setPresentMode(physicalDevice);
        this->setExtent(width, height);

        std::uint32_t imageCount = this->m_surface->getSurfaceCapabilities().minImageCount + 1;
        imageCount = std::clamp(imageCount, 1U, this->m_surface->getSurfaceCapabilities().maxImageCount);

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = this->m_surface->getUnderlyingVkType();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = this->m_surface->getSurfaceFormat().format;
        createInfo.imageColorSpace = this->m_surface->getSurfaceFormat().colorSpace;
        createInfo.imageExtent = this->m_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = this->m_presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;
        createInfo.preTransform = this->m_surface->getSurfaceCapabilities().currentTransform;

        if (this->m_logicalDevice->getQueueFamilyIndices().graphicsFamily != this->m_logicalDevice->getQueueFamilyIndices().presentFamily)
        {
            std::uint32_t queueFamilyIndices[] =
            {
                this->m_logicalDevice->getQueueFamilyIndices().graphicsFamily.value(),
                this->m_logicalDevice->getQueueFamilyIndices().presentFamily.value()
            };

            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_CHECK(vkCreateSwapchainKHR(this->m_logicalDevice->getUnderlyingVkType(), &createInfo, nullptr, &this->m_type));
        this->createImageViews();

        DEBUG_LOG("Vulkan swapchain created.", Log::INFO, Log::ALL);
    }

    void Swapchain::createImageViews() noexcept
    {
        std::uint32_t imageCount;
        vkGetSwapchainImagesKHR(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, &imageCount, nullptr);

        this->m_images.resize(imageCount);
        this->m_imageViews.resize(imageCount);

        vkGetSwapchainImagesKHR(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, &imageCount, this->m_images.data());

        //TODO:
        //Image class
        for (std::size_t i = 0; i < this->m_images.size(); ++i)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = this->m_images.at(i);
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = this->m_surface->getSurfaceFormat().format;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            VK_CHECK(vkCreateImageView(this->m_logicalDevice->getUnderlyingVkType(), &createInfo, nullptr, &this->m_imageViews[i]));
        }
    }

    void Swapchain::destroy() noexcept
    {
        for (const VkImageView& imageView : this->m_imageViews)
		    vkDestroyImageView(this->m_logicalDevice->getUnderlyingVkType(), imageView, nullptr);

        vkDestroySwapchainKHR(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, nullptr);

        DEBUG_LOG("Vulkan swapchain destroyed.", Log::INFO, Log::ALL);
    }

    const VkResult Swapchain::acquireNextImage(std::uint32_t* imageIndex, const std::uint64_t timeout, const std::uint32_t frameIndex) noexcept
    {
        VK_CHECK(vkWaitForFences(this->m_logicalDevice->getUnderlyingVkType(), 1, &this->m_frameSyncObjects.at(frameIndex).getInFlightFence(), VK_TRUE, timeout));

        const VkResult result = vkAcquireNextImageKHR(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, timeout,
            this->m_frameSyncObjects.at(frameIndex).getImageAvailableSemaphore(), VK_NULL_HANDLE, imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR))
            return result;

        vkResetFences(this->m_logicalDevice->getUnderlyingVkType(), 1, &this->m_frameSyncObjects.at(frameIndex).getInFlightFence());

        return result;
    }

    const VkResult Swapchain::present(const std::uint32_t* imageIndex, const std::uint32_t frameIndex) noexcept
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &this->m_frameSyncObjects.at(frameIndex).getRenderFinishedSemaphore();
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &this->m_type;
        presentInfo.pImageIndices = imageIndex;

        return vkQueuePresentKHR(this->m_logicalDevice->getPresentQueue(), &presentInfo);
    }

    const VkResult Swapchain::submit(CommandBuffer& commandBuffer, const std::uint32_t frameIndex) noexcept
    {
        static const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &this->m_frameSyncObjects.at(frameIndex).getImageAvailableSemaphore();
        submitInfo.pWaitDstStageMask = &waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.getUnderlyingVkType();
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &this->m_frameSyncObjects.at(frameIndex).getRenderFinishedSemaphore();

        return vkQueueSubmit(this->m_logicalDevice->getGraphicsQueue(), 1, &submitInfo, this->m_frameSyncObjects.at(frameIndex).getInFlightFence());
    }

    const VkFormat& Swapchain::getFormat() const noexcept
    {
        return this->m_surface->getSurfaceFormat().format;
    }

    const VkExtent2D& Swapchain::getExtent() const noexcept
    {
        return this->m_extent;
    }

    const std::vector<VkImageView>& Swapchain::getImageViews() const noexcept
    {
        return this->m_imageViews;
    }

    void Swapchain::setPresentMode(PhysicalDevice& physicalDevice) noexcept
    {
        std::uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.getUnderlyingVkType(), this->m_surface->getUnderlyingVkType(), &presentModeCount, nullptr);
        this->m_presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.getUnderlyingVkType(), this->m_surface->getUnderlyingVkType(), &presentModeCount, this->m_presentModes.data());

        for (const VkPresentModeKHR& mode : this->m_presentModes)
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
                this->m_presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

        this->m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
    }

    void Swapchain::setExtent(const std::uint32_t width, const std::uint32_t height) noexcept
    {
        const auto& surfaceCapabilities = this->m_surface->getSurfaceCapabilities();
        
        if (surfaceCapabilities.currentExtent.width  != std::numeric_limits<uint32_t>::max() &&
            surfaceCapabilities.currentExtent.height != std::numeric_limits<uint32_t>::max())
            this->m_extent = surfaceCapabilities.currentExtent;
        else
        {
            VkExtent2D actualExtent = { width, height };

            actualExtent.width = std::clamp(actualExtent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

            this->m_extent = actualExtent;
        }
    }
}