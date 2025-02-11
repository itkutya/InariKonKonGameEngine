#pragma once

#include <optional>

#include "InariKonKon/Renderer/Vulkan/Surface.hpp"

namespace ikk
{
    class LogicalDevice : public VkType<VkDevice>
    {
        struct QueueFamilyIndices
        {
            std::optional<std::uint32_t> graphicsFamily;
            std::optional<std::uint32_t> presentFamily;
            std::optional<std::uint32_t> computeFamily;
            std::optional<std::uint32_t> transferFamily;

            inline const bool isComplete() const noexcept
            {
                return  graphicsFamily.has_value()  &&
                        presentFamily.has_value()   &&
                        computeFamily.has_value()   &&
                        transferFamily.has_value();
            }
        };
    public:
        LogicalDevice(Instance& instance, PhysicalDevice& physicalDevice, Surface& surface);

        ~LogicalDevice() noexcept;

        LogicalDevice(const LogicalDevice&) noexcept = default;
        LogicalDevice(LogicalDevice&&) noexcept = default;

        LogicalDevice& operator=(const LogicalDevice&) noexcept = default;
        LogicalDevice& operator=(LogicalDevice&&) noexcept = default;

        const QueueFamilyIndices& getQueueFamilyIndices() const noexcept;
        
        template<class Self>
        [[nodiscard]] inline auto&& getGraphicsQueue(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_graphicsQueue;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getPresentQueue(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_presentQueue;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getComputeQueue(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_computeQueue;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getTransferQueue(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_transferQueue;
        }
    private:
        VkPhysicalDeviceFeatures m_enabledFeatures = {};

        VkQueueFlags m_supportedQueues = {};
        QueueFamilyIndices m_queueFamilies = {};

        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        VkQueue m_presentQueue = VK_NULL_HANDLE;
        VkQueue m_computeQueue = VK_NULL_HANDLE;
        VkQueue m_transferQueue = VK_NULL_HANDLE;

        void createQueueIndices(PhysicalDevice& physicalDevice, Surface& surface);
        void createLogicalDevice(Instance& instance, PhysicalDevice& physicalDevice);
    };
}