#pragma once

#include <vector>

#include "InariKonKon/Graphics/Renderer/Vulkan/Buffer/Buffer.hpp"

#include "InariKonKon/Graphics/Vertex/Vertex.hpp"

namespace ikk
{
    class VertexBuffer : public Buffer
    {
    public:
        //TODO:
        //Specify what type of vertexbuffer u want for example normal vertex with coords/colors, etc...
        VertexBuffer(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, const std::vector<UIVertex>& vertecies) noexcept;
        VertexBuffer(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, const std::vector<UIVertex>& vertecies, CommandPool& commandPool) noexcept;

        ~VertexBuffer() noexcept = default;

        VertexBuffer(const VertexBuffer&) noexcept = default;
        VertexBuffer(VertexBuffer&&) noexcept = default;

        VertexBuffer& operator=(const VertexBuffer&) noexcept = default;
        VertexBuffer& operator=(VertexBuffer&&) noexcept = default;
    private:
    };
}