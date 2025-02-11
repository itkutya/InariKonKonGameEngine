#include "InariKonKon/Renderer/Vulkan/Buffer/Buffer.hpp"

#include <cstring>
#include <array>

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    Buffer::Buffer(LogicalDevice& logicalDevice) noexcept : m_logicalDevice(&logicalDevice)
    {
    }

    Buffer::~Buffer() noexcept
    {
        this->free();
    }

    void Buffer::create(PhysicalDevice &physicalDevice, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties)
    {
        this->m_physicalDevice = &physicalDevice;
        this->m_size = size;
        this->m_usage = usage;
        this->m_properties = properties;

        if (this->m_properties & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT && this->m_commandPool == nullptr)
            throw std::runtime_error("Creating a device local buffer requiers a commandPool!");
        
        std::array queueFamilies = {
                                    this->m_logicalDevice->getQueueFamilyIndices().graphicsFamily.value(),
                                    this->m_logicalDevice->getQueueFamilyIndices().presentFamily.value(),
                                    this->m_logicalDevice->getQueueFamilyIndices().computeFamily.value(),
                                    this->m_logicalDevice->getQueueFamilyIndices().transferFamily.value()
                                   };

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = this->m_size;
        bufferInfo.usage = this->m_usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        //TODO:
        //Sharing mode is ignored couse it is VK_SHARING_MODE_EXCLUSIVE
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferInfo.queueFamilyIndexCount = static_cast<std::uint32_t>(queueFamilies.size());
        bufferInfo.pQueueFamilyIndices = queueFamilies.data();

        VK_CHECK(vkCreateBuffer(this->m_logicalDevice->getUnderlyingVkType(), &bufferInfo, nullptr, &this->m_type));

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = this->findMemoryType(physicalDevice, memRequirements.memoryTypeBits);

        VK_CHECK(vkAllocateMemory(this->m_logicalDevice->getUnderlyingVkType(), &allocInfo, nullptr, &this->m_vertexBufferMemory));

        VK_CHECK(vkBindBufferMemory(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, this->m_vertexBufferMemory, 0));
    }

    void Buffer::create(PhysicalDevice& physicalDevice, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, CommandPool* commandPool, const void* data)
    {
        this->m_commandPool = commandPool;

        if (properties & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        {
            Buffer stagingBuffer{ *this->m_logicalDevice };
            stagingBuffer.create(physicalDevice, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            stagingBuffer.update(data);

            this->create(physicalDevice, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, properties);
        
            copyBuffer(stagingBuffer, *this);
        }
        else
        {
            this->create(physicalDevice, size, usage, properties);
            this->update(data);
        }
    }

    void Buffer::free() noexcept
    {
        vkDeviceWaitIdle(this->m_logicalDevice->getUnderlyingVkType());
            
        vkDestroyBuffer(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, nullptr);
        vkFreeMemory(this->m_logicalDevice->getUnderlyingVkType(), this->m_vertexBufferMemory, nullptr);
    }

    void Buffer::update(const void* data)
    {
        void* transfer;
        VK_CHECK(vkMapMemory(this->m_logicalDevice->getUnderlyingVkType(), this->m_vertexBufferMemory, 0, this->m_size, 0, &transfer));
        std::memcpy(transfer, data, this->m_size);
        vkUnmapMemory(this->m_logicalDevice->getUnderlyingVkType(), this->m_vertexBufferMemory);
    }

    const std::uint32_t Buffer::findMemoryType(PhysicalDevice &physicalDevice, const std::uint32_t typeFilter) const
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice.getUnderlyingVkType(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & this->m_properties) == this->m_properties)
                return i;

        throw std::runtime_error("Failed to find suitable memory type!");
    }

    void copyBuffer(const Buffer& srcBuffer, Buffer& dstBuffer)
    {
        CommandBuffer commandBuffer{ *dstBuffer.getLogicalDevice(), *dstBuffer.getCommandPool() };

        commandBuffer.startRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VkBufferCopy copyRegion{};
        copyRegion.size = dstBuffer.getMemorySize();
        vkCmdCopyBuffer(commandBuffer.getUnderlyingVkType(), srcBuffer.getUnderlyingVkType(), dstBuffer.getUnderlyingVkType(), 1, &copyRegion);

        commandBuffer.endRecording();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.getUnderlyingVkType();

        vkQueueSubmit(dstBuffer.getLogicalDevice()->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(dstBuffer.getLogicalDevice()->getGraphicsQueue());
    }
}