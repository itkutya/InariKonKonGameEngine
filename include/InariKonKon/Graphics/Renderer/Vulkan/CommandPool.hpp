#pragma once

#include "InariKonKon/Graphics/Renderer/Vulkan/LogicalDevice.hpp"

namespace ikk
{
    class CommandPool final : public VkType<VkCommandPool>
    {
    public:
        CommandPool(LogicalDevice& logicalDevice) noexcept;

        ~CommandPool() noexcept;

        CommandPool(const CommandPool&) noexcept = default;
        CommandPool(CommandPool&&) noexcept = default;

        CommandPool& operator=(const CommandPool&) noexcept = default;
        CommandPool& operator=(CommandPool&&) noexcept = default;
    private:
        LogicalDevice* m_logicalDevice = nullptr;
    };
}