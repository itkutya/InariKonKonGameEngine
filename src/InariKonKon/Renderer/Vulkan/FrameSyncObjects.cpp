#include "InariKonKon/Renderer/Vulkan/FrameSyncObjects.hpp"

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    FrameSyncObjects::FrameSyncObjects(LogicalDevice &logicalDevice)
    try : m_logicalDevice(&logicalDevice)
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VK_CHECK(vkCreateSemaphore(this->m_logicalDevice->getUnderlyingVkType(), &semaphoreInfo,
            nullptr, &this->m_imageAvailable));
        VK_CHECK(vkCreateSemaphore(this->m_logicalDevice->getUnderlyingVkType(), &semaphoreInfo,
            nullptr, &this->m_renderFinished));
        VK_CHECK(vkCreateFence(this->m_logicalDevice->getUnderlyingVkType(), &fenceInfo,
            nullptr, &this->m_inFlight));
    }
    catch(const std::exception& e)
    {
        std::print("{}", e.what());
    }
    
    FrameSyncObjects::~FrameSyncObjects() noexcept
    {
        vkDeviceWaitIdle(this->m_logicalDevice->getUnderlyingVkType());
        
        vkDestroySemaphore(this->m_logicalDevice->getUnderlyingVkType(), this->m_imageAvailable, nullptr);
        vkDestroySemaphore(this->m_logicalDevice->getUnderlyingVkType(), this->m_renderFinished, nullptr);
        vkDestroyFence(this->m_logicalDevice->getUnderlyingVkType(), this->m_inFlight, nullptr);
    }
}