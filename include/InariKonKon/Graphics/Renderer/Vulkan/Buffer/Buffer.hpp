#pragma once

#include "InariKonKon/Graphics/Renderer/Vulkan/LogicalDevice.hpp"
#include "InariKonKon/Graphics/Renderer/Vulkan/CommandBuffer.hpp"

namespace ikk
{
    class Buffer : public VkType<VkBuffer>
    {
    public:
        Buffer(LogicalDevice& logicalDevice) noexcept;

        virtual ~Buffer() noexcept;

        Buffer(const Buffer&) noexcept = default;
        Buffer(Buffer&&) noexcept = default;

        Buffer& operator=(const Buffer&) noexcept = default;
        Buffer& operator=(Buffer&&) noexcept = default;

        virtual void create(PhysicalDevice& physicalDevice, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties) final;
        virtual void create(PhysicalDevice& physicalDevice, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, CommandPool* commandPool, const void* data) final;
        virtual void free() noexcept final;

        virtual void update(const void* data);
    private:
        LogicalDevice* m_logicalDevice = nullptr;
        CommandPool* m_commandPool = nullptr;

        VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;

        VkDeviceSize m_size = 0;

        VkBufferUsageFlags m_usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        VkMemoryPropertyFlags m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        const std::uint32_t findMemoryType(PhysicalDevice& physicalDevice, const std::uint32_t typeFilter) const;

        friend void copyBuffer(const Buffer& srcBuffer, Buffer& dstBuffer);
    };

    void copyBuffer(const Buffer& srcBuffer, Buffer& dstBuffer);
}