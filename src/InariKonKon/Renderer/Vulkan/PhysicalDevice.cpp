#include "InariKonKon/Renderer/Vulkan/PhysicalDevice.hpp"

#include <algorithm>
#include <string>
#include <set>

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    PhysicalDevice::PhysicalDevice(Instance& instance)
    try
    {
        std::uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance.getUnderlyingVkType(), &deviceCount, nullptr);

        if (deviceCount == 0)
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");

        std::vector<VkPhysicalDevice> devices { deviceCount };
        vkEnumeratePhysicalDevices(instance.getUnderlyingVkType(), &deviceCount, devices.data());

        //TODO:
        //Allow user to configure graphics preference
        this->m_type = this->pickBestPhysicalDevice(devices);

        if (this->m_type == VK_NULL_HANDLE)
            throw std::runtime_error("Failed to find a suitable GPU!");

        vkGetPhysicalDeviceProperties(this->m_type, &this->m_properties);
        vkGetPhysicalDeviceFeatures(this->m_type, &this->m_features);
        vkGetPhysicalDeviceMemoryProperties(this->m_type, &this->m_memoryProperties);
        this->m_msaaSamples = this->getMaxUsableSampleCount();
    }
    catch(const std::exception& e)
    {
        std::print("{}\n", e.what());
    }

    const VkPhysicalDeviceProperties& PhysicalDevice::getDeviceProperties() const noexcept
    {
        return this->m_properties;
    }

    const VkPhysicalDeviceFeatures& PhysicalDevice::getDeviceFeatures() const noexcept
    {   
        return this->m_features;
    }

    const VkPhysicalDeviceMemoryProperties& PhysicalDevice::getDeviceMemoryProperties() const noexcept
    {
        return this->m_memoryProperties;
    }

    const VkSampleCountFlagBits& PhysicalDevice::getDeviceMSAASamples() const noexcept
    {
        return this->m_msaaSamples;
    }

    const std::vector<const char *>& PhysicalDevice::getDeviceEstensions() const noexcept
    {
        return this->m_deviceExtensions;
    }

    VkPhysicalDevice PhysicalDevice::pickBestPhysicalDevice(const std::vector<VkPhysicalDevice>& devices) const noexcept
    {
        std::vector<std::pair<std::uint64_t, VkPhysicalDevice>> rankedDevices;
        rankedDevices.reserve(devices.size());

        for (const VkPhysicalDevice& device : devices)
            rankedDevices.emplace_back(std::make_pair(this->scorePhysicalDevice(device), device));

        std::ranges::sort(rankedDevices, [](const auto& left, const auto& right){ return left.first > right.first; });

        if (rankedDevices.back().first > 0)
            return rankedDevices.back().second;

        return VK_NULL_HANDLE;
    }

    const std::uint64_t PhysicalDevice::scorePhysicalDevice(const VkPhysicalDevice &device) const noexcept
    {
        std::uint64_t score = 0;

        std::uint32_t extensionPropertyCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, nullptr);
        std::vector<VkExtensionProperties> extensionProperties{ extensionPropertyCount };
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, extensionProperties.data());

        const std::vector<const char*> deviceExtensions = this->m_deviceExtensions;
        std::set<std::string> requireddeviceExtensions{ deviceExtensions.begin(), deviceExtensions.end() };

        for (const char* deviceExtension : deviceExtensions)
            requireddeviceExtensions.erase(deviceExtension);
        
        if (!requireddeviceExtensions.empty())
            return 0;

        //VkPhysicalDeviceFeatures physicalDeviceFeatures;
        //vkGetPhysicalDeviceFeatures(device, &physicalDeviceFeatures);
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

        if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            score += 1000;

        score += physicalDeviceProperties.limits.maxImageDimension2D;

        VkMemoryHeap* heapsPointer = memoryProperties.memoryHeaps;
        std::vector<VkMemoryHeap> heaps{ heapsPointer, heapsPointer + memoryProperties.memoryHeapCount };

        for (const VkMemoryHeap& heap : heaps)
            if (heap.flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
                score += static_cast<std::uint64_t>(heap.size);

        return score;
    }

    VkSampleCountFlagBits PhysicalDevice::getMaxUsableSampleCount() const noexcept
    {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(this->m_type, &physicalDeviceProperties);

        const VkSampleCountFlags counts =
        std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);

        for (const VkSampleCountFlagBits& sampleFlag : {
                                                        VK_SAMPLE_COUNT_64_BIT,
                                                        VK_SAMPLE_COUNT_32_BIT,
                                                        VK_SAMPLE_COUNT_16_BIT,
                                                        VK_SAMPLE_COUNT_8_BIT,
                                                        VK_SAMPLE_COUNT_4_BIT,
                                                        VK_SAMPLE_COUNT_2_BIT
                                                       })
            if (counts & sampleFlag)
                return sampleFlag;

        return VK_SAMPLE_COUNT_1_BIT;
    }
}