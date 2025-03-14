#pragma once

#include <string_view>
#include <vector>

#include "InariKonKon/Graphics/Renderer/Vulkan/VulkanTypeBase.hpp"

struct GLFWwindow;

namespace ikk
{
    class Instance final : public VkType<VkInstance>
    {
    public:
        Instance(const std::u8string_view title);

        ~Instance() noexcept;

        Instance(const Instance&) noexcept = default;
        Instance(Instance&&) noexcept = default;

        Instance& operator=(const Instance&) noexcept = default;
        Instance& operator=(Instance&&) noexcept = default;

        [[nodiscard]] static const std::vector<const char*> getValidationLayers() noexcept;
    private:
        [[nodiscard]] static const std::vector<const char*> getRequiredExtensions() noexcept;
    };
}