#ifndef IKK_VERTEX_HPP
#define IKK_VERTEX_HPP

#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "InariKonKon/Graphics/Color/Color.hpp"

namespace ikk
{
    struct VertexAttributes
    {
        enum struct Format
        {
            Float, Vec2, Vec3, Vec4
        };

        std::uint32_t binding = 0;
        std::uint32_t location = 0;
        Format format = Format::Float;
        std::uint32_t offset = 0;
    };

    struct VertexInfo
    {
        enum struct InputRate
        {
            Per_Vertex, Per_Instance
        };

        std::uint32_t binding = 0;
        std::uint32_t stride = 0;
        InputRate inputRate = InputRate::Per_Vertex;
        std::vector<VertexAttributes> attributes{};
    };

    struct UIVertex
    {
        glm::vec2 position  { 0.f, 0.f };
        Color color         { 0.f, 0.f, 0.f, 1.f };
    };
    
    struct ObjVertex
    {
        glm::vec3 position      { 0.f, 0.f, 0.f };
        glm::vec2 texCoord      { 0.f, 0.f };
        Color color             { 0.f, 0.f, 0.f, 1.f };
    };
}

#endif