#pragma once

#include <array>
#include <cstddef>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "vulkan/vulkan.h"

#include "InariKonKon/Model/Color.hpp"

namespace ikk
{
    //TODO:
    //Make it more flexible...
    //& hide from user the binding stuff somehow...
    struct Vertex final
    {
        glm::vec3 m_position{ 0.f, 0.f, 0.f };
        Color m_color{ 0.f, 0.f, 0.f, 0.f };
        //glm::vec2 m_textureCoord{ 0.f, 0.f };

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, m_position);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, m_color);

            return attributeDescriptions;
        }
    };
}