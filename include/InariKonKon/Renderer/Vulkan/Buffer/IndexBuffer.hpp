#pragma once

#include <vector>

#include "InariKonKon/Renderer/Vulkan/Buffer/Buffer.hpp"

namespace ikk
{
    class IndexBuffer : public Buffer
    {
    public:
        //TODO:
        //Specify what type of indexbuffer u want for example std::uin16_t, etc...
        IndexBuffer(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, const std::vector<std::uint32_t>& indecies) noexcept;
        IndexBuffer(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, const std::vector<std::uint32_t>& indecies, CommandPool& commandPool) noexcept;

        ~IndexBuffer() noexcept = default;

        IndexBuffer(const IndexBuffer&) noexcept = default;
        IndexBuffer(IndexBuffer&&) noexcept = default;

        IndexBuffer& operator=(const IndexBuffer&) noexcept = default;
        IndexBuffer& operator=(IndexBuffer&&) noexcept = default;
    private:
    };
}