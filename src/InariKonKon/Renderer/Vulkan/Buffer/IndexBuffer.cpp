#include "InariKonKon/Renderer/Vulkan/Buffer/IndexBuffer.hpp"

#include "InariKonKon/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    IndexBuffer::IndexBuffer(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, const std::vector<std::uint32_t>& indicies) noexcept
        : Buffer(logicalDevice)
    {
        this->create(physicalDevice, sizeof(indicies.at(0)) * indicies.size(),
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        this->update((const void*)indicies.data());
    }

    IndexBuffer::IndexBuffer(LogicalDevice &logicalDevice, PhysicalDevice &physicalDevice, const std::vector<std::uint32_t> &indecies, CommandPool &commandPool) noexcept
        : Buffer(logicalDevice)
    {
        this->create(physicalDevice, sizeof(indecies.at(0)) * indecies.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &commandPool, (const void*)indecies.data());
    }
}

