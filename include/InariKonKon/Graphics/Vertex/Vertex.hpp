#pragma once

#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "InariKonKon/Graphics/Color/Color.hpp"

namespace ikk
{   
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

    struct VertexAttributes
    {
        std::uint32_t binding = 0;
        std::uint32_t location = 0;
        //TODO:
        //Format...
        std::uint32_t offset = 0;
    };

    struct VertexInfo
    {
        std::uint32_t binding = 0;
        std::uint32_t stride = 0;
        //TODO:
        //Input rate...
        std::vector<VertexAttributes> attributes{};
    };
}