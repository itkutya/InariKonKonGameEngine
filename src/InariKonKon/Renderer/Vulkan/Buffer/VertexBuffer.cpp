#include "InariKonKon/Renderer/Vulkan/Buffer/VertexBuffer.hpp"

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    VertexBuffer::VertexBuffer(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, const std::vector<Vertex>& vertecies) noexcept
        : Buffer(logicalDevice)
    {
        this->create(physicalDevice, sizeof(vertecies.at(0)) * vertecies.size(),
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        this->update((const void*)vertecies.data());
    }

    VertexBuffer::VertexBuffer(LogicalDevice &logicalDevice, PhysicalDevice &physicalDevice, const std::vector<Vertex> &vertecies, CommandPool &commandPool) noexcept
        : Buffer(logicalDevice)
    {
        this->create(physicalDevice, sizeof(vertecies.at(0)) * vertecies.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &commandPool, (const void*)vertecies.data());
    }
}