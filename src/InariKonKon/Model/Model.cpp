#include "InariKonKon/Model/Model.hpp"

namespace ikk
{
    Model::Model(const std::vector<Vertex>& vertecies) noexcept : m_vertecies(vertecies)
    {
    }

    Model::Model(const std::vector<Vertex>& vertecies, const std::vector<std::uint32_t>& indicies) noexcept : m_vertecies(vertecies), m_indicies(indicies)
    {
    }

    std::vector<Vertex> &Model::getVertecies() noexcept
    {
        return this->m_vertecies;
    }

    const std::vector<Vertex>& Model::getVertecies() const noexcept
    {
        return this->m_vertecies;
    }
}