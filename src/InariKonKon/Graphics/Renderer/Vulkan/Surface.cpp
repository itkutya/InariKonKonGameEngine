#include "InariKonKon/Graphics/Renderer/Vulkan/Surface.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    Surface::Surface(Instance& instance, GLFWwindow* window) noexcept : m_instance(&instance), m_window(window)
    {
        VK_CHECK(glfwCreateWindowSurface(this->m_instance->getUnderlyingVkType(), this->m_window, nullptr, &this->m_type));

        DEBUG_LOG("Vulkan surface created.", Log::INFO, Log::ALL);
    }

    Surface::~Surface() noexcept
    {
        vkDestroySurfaceKHR(this->m_instance->getUnderlyingVkType(), this->m_type, nullptr);

        DEBUG_LOG("Vulkan surface destroyed.", Log::INFO, Log::ALL);
    }

    const VkSurfaceCapabilitiesKHR& Surface::getSurfaceCapabilities() const noexcept
    {
        return this->m_capabilities;
    }

    const VkSurfaceFormatKHR& Surface::getSurfaceFormat() const noexcept
    {
        return this->m_format;
    }

    void Surface::chooseSurfaceFormat(PhysicalDevice& physicalDevice)
    {
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.getUnderlyingVkType(), this->m_type, &this->m_capabilities));

        std::uint32_t surfaceFormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.getUnderlyingVkType(), this->m_type, &surfaceFormatCount, nullptr);
        this->m_surfaceFormats.resize(static_cast<std::size_t>(surfaceFormatCount));
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.getUnderlyingVkType(), this->m_type, &surfaceFormatCount, this->m_surfaceFormats.data());

        if (surfaceFormatCount > 0)
        {
            //TODO:
            //Add the ability to chose whaterver they want
            for (const VkSurfaceFormatKHR& format : this->m_surfaceFormats)
                if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                    this->m_format = format;

            this->m_format = this->m_surfaceFormats[0];
        }
        else
            throw std::runtime_error("No valid surface format was found!");
    }
}