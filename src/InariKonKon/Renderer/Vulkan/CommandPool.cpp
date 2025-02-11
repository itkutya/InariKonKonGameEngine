#include "InariKonKon/Renderer/Vulkan/CommandPool.hpp"

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    CommandPool::CommandPool(LogicalDevice& logicalDevice)
    try : m_logicalDevice(&logicalDevice)
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = this->m_logicalDevice->getQueueFamilyIndices().graphicsFamily.value();

        VK_CHECK(vkCreateCommandPool(this->m_logicalDevice->getUnderlyingVkType(), &poolInfo,
            nullptr, &this->m_type));
    }
    catch(const std::exception& e)
    {
        std::print("{}\n", e.what());
    }

    CommandPool::~CommandPool() noexcept
    {
        vkDestroyCommandPool(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, nullptr);
    }
}