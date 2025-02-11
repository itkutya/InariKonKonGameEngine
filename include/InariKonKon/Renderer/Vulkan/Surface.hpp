#pragma once

#include "InariKonKon/Renderer/Vulkan/PhysicalDevice.hpp"

struct GLFWwindow;

namespace ikk
{
    class Surface : public VkType<VkSurfaceKHR>
    {
    public:
        Surface(Instance& instance, GLFWwindow* window);

        ~Surface() noexcept;

        Surface(const Surface&) noexcept = default;
        Surface(Surface&&) noexcept = default;

        Surface& operator=(const Surface&) noexcept = default;
        Surface& operator=(Surface&&) noexcept = default;

        const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities() const noexcept;
        const VkSurfaceFormatKHR& getSurfaceFormat() const noexcept;

        void chooseSurfaceFormat(PhysicalDevice& physicalDevice);
    private:
        Instance* m_instance;
        GLFWwindow* m_window;

	    VkSurfaceCapabilitiesKHR m_capabilities = {};
	    VkSurfaceFormatKHR m_format = {};
        std::vector<VkSurfaceFormatKHR> m_surfaceFormats = {};
    };
}