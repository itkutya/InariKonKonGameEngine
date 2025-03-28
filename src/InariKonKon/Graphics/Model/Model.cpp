#include "InariKonKon/Graphics/Model/Model.hpp"

namespace ikk
{
    ModelBase::ModelBase(const Shader& fragment, const Shader& vertex, const VertexInfo info) noexcept
        : m_fragment(fragment), m_vertex(vertex), m_info(info)
    {
    }

    const Shader& ModelBase::getFragmentShader() const noexcept
    {
        return this->m_fragment;
    }
    
    const Shader& ModelBase::getVertexShader() const noexcept
    {
        return this->m_vertex;
    }

    const VertexInfo& ModelBase::getVertexInfo() const noexcept
    {
        return this->m_info;
    }
}