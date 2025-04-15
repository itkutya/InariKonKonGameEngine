#ifndef IKK_VERTEX_HPP
#define IKK_VERTEX_HPP

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
}

#endif