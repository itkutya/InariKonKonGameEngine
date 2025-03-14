#include "InariKonKon/Graphics/Renderer/Vulkan/PhysicalDevice.hpp"

#include <algorithm>
#include <string>
#include <set>

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

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
        //Allow users to add their own extensions if needed...
        this->setDeviceExtensions();
        //TODO:
        //Allow users to configure graphics preference
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
        Log(e.what(), Log::FATAL, Log::ALL);
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

    void PhysicalDevice::setDeviceExtensions() noexcept
    {
        this->m_deviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    VkPhysicalDevice PhysicalDevice::pickBestPhysicalDevice(const std::vector<VkPhysicalDevice>& devices) const noexcept
    {
        struct DeviceScores
        {
            std::uint64_t score = 0;
            VkPhysicalDevice device = VK_NULL_HANDLE;
        };

        std::vector<DeviceScores> rankedDevices{ devices.size() };

        for (std::size_t i = 0; i < devices.size(); ++i)
            rankedDevices.at(i) = DeviceScores({ .score = this->scorePhysicalDevice(devices.at(i)), .device = devices.at(i) });

        std::ranges::sort(rankedDevices, [](const DeviceScores& left, const DeviceScores& right){ return left.score > right.score; });

        if (rankedDevices.back().score > 0)
            return rankedDevices.back().device;

        return VK_NULL_HANDLE;
    }

    const std::uint64_t PhysicalDevice::scorePhysicalDevice(const VkPhysicalDevice& device) const noexcept
    {
        std::uint64_t score = 0;

        std::uint32_t extensionPropertyCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, nullptr);
        std::vector<VkExtensionProperties> extensionProperties{ extensionPropertyCount };
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, extensionProperties.data());

        std::set<std::string> requireddeviceExtensions{ this->m_deviceExtensions.begin(), this->m_deviceExtensions.end() };

        for (const char* deviceExtension : this->m_deviceExtensions)
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
                score += U64(heap.size);

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