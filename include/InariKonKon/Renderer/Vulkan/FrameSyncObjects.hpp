#pragma once

#include "InariKonKon/Renderer/Vulkan/LogicalDevice.hpp"

namespace ikk
{
    class FrameSyncObjects
    {
    public:
        FrameSyncObjects(LogicalDevice& logicalDevice);

        ~FrameSyncObjects() noexcept;

        FrameSyncObjects(const FrameSyncObjects&) noexcept = default;
        FrameSyncObjects(FrameSyncObjects&&) noexcept = default;

        FrameSyncObjects& operator=(const FrameSyncObjects&) noexcept = default;
        FrameSyncObjects& operator=(FrameSyncObjects&&) noexcept = default;

        template<class Self>
        [[nodiscard]] inline auto&& getImageAvailableSemaphore(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_imageAvailable;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getRenderFinishedSemaphore(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_renderFinished;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getInFlightFence(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_inFlight;
        }
    private:
        LogicalDevice* m_logicalDevice = nullptr;
        
        VkSemaphore m_imageAvailable = VK_NULL_HANDLE;
        VkSemaphore m_renderFinished = VK_NULL_HANDLE;
        VkFence m_inFlight = VK_NULL_HANDLE;
    };
}