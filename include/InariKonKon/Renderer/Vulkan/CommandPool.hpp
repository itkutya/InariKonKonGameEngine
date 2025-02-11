#pragma once

#include "InariKonKon/Renderer/Vulkan/LogicalDevice.hpp"

namespace ikk
{
    class CommandPool : public VkType<VkCommandPool>
    {
    public:
        CommandPool(LogicalDevice& logicalDevice);

        ~CommandPool() noexcept;

        CommandPool(const CommandPool&) noexcept = default;
        CommandPool(CommandPool&&) noexcept = default;

        CommandPool& operator=(const CommandPool&) noexcept = default;
        CommandPool& operator=(CommandPool&&) noexcept = default;
    private:
        LogicalDevice* m_logicalDevice;
    };
}