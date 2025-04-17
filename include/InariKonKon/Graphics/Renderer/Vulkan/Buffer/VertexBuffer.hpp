#ifndef IKK_VERTEX_BUFFER_HPP
#define IKK_VERTEX_BUFFER_HPP

#include "InariKonKon/Graphics/Renderer/Vulkan/Buffer/Buffer.hpp"

#include "InariKonKon/Graphics/Vertex/Vertex.hpp"

namespace ikk
{
    template<class VertexType>
    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, const std::vector<VertexType>& vertecies) noexcept;
        VertexBuffer(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, const std::vector<VertexType>& vertecies, CommandPool& commandPool) noexcept;

        ~VertexBuffer() noexcept = default;

        VertexBuffer(const VertexBuffer&) noexcept = default;
        VertexBuffer(VertexBuffer&&) noexcept = default;

        VertexBuffer& operator=(const VertexBuffer&) noexcept = default;
        VertexBuffer& operator=(VertexBuffer&&) noexcept = default;
    private:
    };

    template<class VertexType>
    VertexBuffer<VertexType>::VertexBuffer(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, const std::vector<VertexType>& vertecies) noexcept
        : Buffer(logicalDevice)
    {
        this->create(physicalDevice, sizeof(vertecies.at(0)) * vertecies.size(),
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        this->update((const void*)vertecies.data());
    }

    template<class VertexType>
    VertexBuffer<VertexType>::VertexBuffer(LogicalDevice &logicalDevice, PhysicalDevice &physicalDevice, const std::vector<VertexType> &vertecies, CommandPool &commandPool) noexcept
        : Buffer(logicalDevice)
    {
        this->create(physicalDevice, sizeof(vertecies.at(0)) * vertecies.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &commandPool, (const void*)vertecies.data());
    }
}

#endif