#ifndef IKK_SURFACE_HPP
#define IKK_SURFACE_HPP

#include "InariKonKon/Graphics/Renderer/Vulkan/PhysicalDevice.hpp"

struct GLFWwindow;

namespace ikk
{
    class Surface final : public VkType<VkSurfaceKHR>
    {
    public:
        Surface(Instance& instance, GLFWwindow* window) noexcept;

        ~Surface() noexcept;

        Surface(const Surface&) noexcept = default;
        Surface(Surface&&) noexcept = default;

        Surface& operator=(const Surface&) noexcept = default;
        Surface& operator=(Surface&&) noexcept = default;

        const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities() const noexcept;
        const VkSurfaceFormatKHR& getSurfaceFormat() const noexcept;

        void chooseSurfaceFormat(PhysicalDevice& physicalDevice);
    private:
        Instance* m_instance = nullptr;
        GLFWwindow* m_window = nullptr;

	    VkSurfaceCapabilitiesKHR m_capabilities = {};
	    VkSurfaceFormatKHR m_format = {};
        std::vector<VkSurfaceFormatKHR> m_surfaceFormats = {};
    };
}

#endif