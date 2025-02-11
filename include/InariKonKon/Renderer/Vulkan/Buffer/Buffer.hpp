#pragma once

#include "InariKonKon/Renderer/Vulkan/LogicalDevice.hpp"
#include "InariKonKon/Renderer/Vulkan/CommandBuffer.hpp"

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

        template<class Self>
        [[nodiscard]] inline auto&& getPhysicalDevice(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_physicalDevice;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getLogicalDevice(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_logicalDevice;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getCommandPool(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_commandPool;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getDeviceMemory(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_vertexBufferMemory;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getMemorySize(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_size;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getState(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_state;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getBufferUsageFlags(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_usage;
        }

        template<class Self>
        [[nodiscard]] inline auto&& getMemoryPropertyFlags(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_properties;
        }
    private:
        PhysicalDevice* m_physicalDevice = nullptr;
        LogicalDevice* m_logicalDevice = nullptr;
        CommandPool* m_commandPool = nullptr;

        VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;

        VkDeviceSize m_size = 0;

        VkBufferUsageFlags m_usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        VkMemoryPropertyFlags m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        const std::uint32_t findMemoryType(PhysicalDevice& physicalDevice, const std::uint32_t typeFilter) const;
    };

    void copyBuffer(const Buffer& srcBuffer, Buffer& dstBuffer);
}