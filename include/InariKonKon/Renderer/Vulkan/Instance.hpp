#pragma once

#include <string_view>
#include <vector>

#include "InariKonKon/Renderer/Vulkan/VulkanTypeBase.hpp"

struct GLFWwindow;

namespace ikk
{
    class Instance final : public VkType<VkInstance>
    {
    public:
        Instance(std::u8string_view title);

        ~Instance() noexcept;

        Instance(const Instance&) noexcept = default;
        Instance(Instance&&) noexcept = default;

        Instance& operator=(const Instance&) noexcept = default;
        Instance& operator=(Instance&&) noexcept = default;

        const std::vector<const char*> getValidationLayers() const noexcept;
    private:
        [[nodiscard]] std::vector<const char*> getRequiredExtensions() const noexcept;
    };
}