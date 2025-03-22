#include "InariKonKon/Graphics/Model/Model.hpp"

namespace ikk
{
    const Shader& ModelBase::getFragmentShader() const noexcept
    {
        return this->m_fragment;
    }
    
    const Shader& ModelBase::getVertexShader() const noexcept
    {
        return this->m_vertex;
    }
}