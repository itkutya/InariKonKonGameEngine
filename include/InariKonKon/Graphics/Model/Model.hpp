#pragma once

#include <vector>

#include "InariKonKon/Graphics/Shader/Shader.hpp"

namespace ikk
{
    //TODO:
    //Type safety...
    template<class VertexType, class IndiciesType = std::uint16_t>
    class Model final
    {
    public:
        Model(const Shader& fragment, const Shader& vertex, const std::vector<VertexType>& vertecies, const std::vector<IndiciesType>& indicies = {});

        ~Model() noexcept = default;

        Model(const Model&) noexcept = default;
        Model(Model&&) noexcept = default;

        Model& operator=(const Model&) noexcept = default;
        Model& operator=(Model&&) noexcept = default;
    private:
        Shader m_fragment;
        Shader m_vertex;
        std::vector<VertexType> m_vertecies;
        std::vector<IndiciesType> m_indicies;
    };

    template <class VertexType, class IndiciesType>
    Model<VertexType, IndiciesType>::Model(const Shader& fragment, const Shader& vertex,
                                           const std::vector<VertexType>& vertecies, const std::vector<IndiciesType>& indicies)
        : m_fragment(fragment), m_vertex(vertex), m_vertecies(vertecies), m_indicies(indicies)
    {
    }
}