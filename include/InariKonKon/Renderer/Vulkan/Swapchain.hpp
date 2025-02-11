#pragma once

#include "InariKonKon/Renderer/Vulkan/LogicalDevice.hpp"
#include "InariKonKon/Renderer/Vulkan/FrameSyncObjects.hpp"

namespace ikk
{
    class CommandBuffer;

    class Swapchain : public VkType<VkSwapchainKHR>
    {
    public:
        Swapchain(const std::uint32_t width, const std::uint32_t height, Surface& surface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, const std::size_t maxFramesInFlight);

        ~Swapchain() noexcept;

        Swapchain(const Swapchain&) noexcept = default;
        Swapchain(Swapchain&&) noexcept = default;

        Swapchain& operator=(const Swapchain&) noexcept = default;
        Swapchain& operator=(Swapchain&&) noexcept = default;

        void create(const std::uint32_t width, const std::uint32_t height, PhysicalDevice& physicalDevice);
        void destroy() noexcept;
        
        [[nodiscard]] const VkResult acquireNextImage(std::uint32_t* imageIndex, const std::uint64_t timeout, const std::uint32_t frameIndex);
        [[nodiscard]] const VkResult present(const std::uint32_t* imageIndex, const std::uint32_t frameIndex) noexcept;
        const VkResult submit(CommandBuffer& commandBuffer, const std::uint32_t frameIndex) noexcept;

        [[nodiscard]] const VkFormat getFormat() const noexcept;
        [[nodiscard]] const VkExtent2D& getExtent() const noexcept;
        [[nodiscard]] const std::vector<VkImageView>& getImageViews() const noexcept;
    private:
        Surface* m_surface = nullptr;
        LogicalDevice* m_logicalDevice = nullptr;

        VkPresentModeKHR m_presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
        VkExtent2D m_extent = { 0, 0 };
        
        std::vector<VkPresentModeKHR> m_presentModes;
        std::vector<VkImage> m_images;
	    std::vector<VkImageView> m_imageViews;

        std::vector<FrameSyncObjects> m_frameSyncObjects;

        void createImageViews();

        void setPresentMode(PhysicalDevice& physicalDevice) noexcept;
        void setExtent(const std::uint32_t width, const std::uint32_t height) noexcept;
    };
}