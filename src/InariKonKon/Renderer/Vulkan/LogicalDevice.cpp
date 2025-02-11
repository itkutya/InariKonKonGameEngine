#include "InariKonKon/Renderer/Vulkan/LogicalDevice.hpp"

#include <set>

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    LogicalDevice::LogicalDevice(Instance& instance, PhysicalDevice& physicalDevice, Surface& surface)
    try
    {
        this->createQueueIndices(physicalDevice, surface);
        this->createLogicalDevice(instance, physicalDevice);
    }
    catch(const std::exception& e)
    {
        std::print("{}\n", e.what());
    }

    LogicalDevice::~LogicalDevice() noexcept
    {
        vkDeviceWaitIdle(this->m_type);
        vkDestroyDevice(this->m_type, nullptr);
    }

    const LogicalDevice::QueueFamilyIndices& LogicalDevice::getQueueFamilyIndices() const noexcept
    {
        return this->m_queueFamilies;
    }

    void LogicalDevice::createQueueIndices(PhysicalDevice &physicalDevice, Surface &surface)
    {
        std::uint32_t deviceQueueFamilyPropertyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.getUnderlyingVkType(),
            &deviceQueueFamilyPropertyCount, nullptr);
        std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties{ deviceQueueFamilyPropertyCount };
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.getUnderlyingVkType(),
            &deviceQueueFamilyPropertyCount, deviceQueueFamilyProperties.data());

        for (std::uint32_t i = 0; i < deviceQueueFamilyPropertyCount; ++i)
        {
            if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                this->m_queueFamilies.graphicsFamily = i;
                this->m_supportedQueues |= VK_QUEUE_GRAPHICS_BIT;
            }

            VkBool32 presentSupport;
            vkGetPhysicalDeviceSurfaceSupportKHR(
                physicalDevice.getUnderlyingVkType(), i, 
                surface.getUnderlyingVkType(), &presentSupport);
            
            if (deviceQueueFamilyProperties[i].queueCount > 0 && presentSupport)
                this->m_queueFamilies.presentFamily = i;

            if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                this->m_queueFamilies.computeFamily = i;
                this->m_supportedQueues |= VK_QUEUE_COMPUTE_BIT;
            }

            if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                this->m_queueFamilies.transferFamily = i;
                this->m_supportedQueues |= VK_QUEUE_TRANSFER_BIT;
            }

            if (this->m_queueFamilies.isComplete())
                break;
        }

        if (!this->m_queueFamilies.graphicsFamily.has_value())
            throw std::runtime_error("Failed to find queue family supporting VK_QUEUE_GRAPHICS_BIT");

        if (!this->m_queueFamilies.presentFamily.has_value())
            throw std::runtime_error("Present queue family does not have presentation support");
    }

    void LogicalDevice::createLogicalDevice(Instance& instance, PhysicalDevice& physicalDevice)
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        float queuePriority = 1.0f;

        std::set queueFamileis = {
                                    this->m_queueFamilies.graphicsFamily.value(),
                                    this->m_queueFamilies.presentFamily.value(),
                                    this->m_queueFamilies.computeFamily.value(),
                                    this->m_queueFamilies.transferFamily.value()
                                 };
        
        for (const std::uint32_t queueFamily : queueFamileis)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.emplace_back(queueCreateInfo);
        }

        //const VkPhysicalDeviceFeatures& physicalDeviceFeatures = physicalDevice.getDeviceFeatures();
        //TODO:
        //Enable them here...
        //this->m_enabledFeatures...

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &this->m_enabledFeatures;
        createInfo.enabledExtensionCount = static_cast<std::uint32_t>(physicalDevice.getDeviceEstensions().size());
        createInfo.ppEnabledExtensionNames = physicalDevice.getDeviceEstensions().data();
        #ifdef IKK_DEBUG
            const std::vector<const char*> validationLayer = instance.getValidationLayers();
            createInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayer.size());
            createInfo.ppEnabledLayerNames = validationLayer.data();
        #else
            createInfo.enabledLayerCount = 0;
        #endif

        VK_CHECK(vkCreateDevice(physicalDevice.getUnderlyingVkType(), &createInfo, nullptr, &this->m_type));

        vkGetDeviceQueue(this->m_type, this->m_queueFamilies.graphicsFamily.value(), 0, &this->m_graphicsQueue);
        vkGetDeviceQueue(this->m_type, this->m_queueFamilies.presentFamily.value(), 0, &this->m_presentQueue);
        vkGetDeviceQueue(this->m_type, this->m_queueFamilies.computeFamily.value(), 0, &this->m_computeQueue);
        vkGetDeviceQueue(this->m_type, this->m_queueFamilies.transferFamily.value(), 0, &this->m_transferQueue);
    }
}