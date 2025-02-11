#pragma once

#include "InariKonKon/Renderer/Vulkan/Instance.hpp"

namespace ikk
{
    class PhysicalDevice : public VkType<VkPhysicalDevice>
    {
    public:
        PhysicalDevice(Instance& instance);

        ~PhysicalDevice() noexcept = default;

        PhysicalDevice(const PhysicalDevice&) noexcept = default;
        PhysicalDevice(PhysicalDevice&&) noexcept = default;

        PhysicalDevice& operator=(const PhysicalDevice&) noexcept = default;
        PhysicalDevice& operator=(PhysicalDevice&&) noexcept = default;

        [[nodiscard]] const VkPhysicalDeviceProperties& getDeviceProperties() const noexcept;
        [[nodiscard]] const VkPhysicalDeviceFeatures& getDeviceFeatures() const noexcept;
        [[nodiscard]] const VkPhysicalDeviceMemoryProperties& getDeviceMemoryProperties() const noexcept;
        [[nodiscard]] const VkSampleCountFlagBits& getDeviceMSAASamples() const noexcept;
        [[nodiscard]] const std::vector<const char*>& getDeviceEstensions() const noexcept;
    private:
        VkPhysicalDeviceProperties m_properties = {};
        VkPhysicalDeviceFeatures m_features = {};
        VkPhysicalDeviceMemoryProperties m_memoryProperties = {};
        VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;

        std::vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        [[nodiscard]] VkPhysicalDevice pickBestPhysicalDevice(const std::vector<VkPhysicalDevice>& devices) const noexcept;
        [[nodiscard]] const std::uint64_t scorePhysicalDevice(const VkPhysicalDevice &device) const noexcept;
        [[nodiscard]] VkSampleCountFlagBits getMaxUsableSampleCount() const noexcept;
    };
}