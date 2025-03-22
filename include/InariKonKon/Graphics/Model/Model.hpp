#pragma once

#include <vector>

#include "InariKonKon/Graphics/Vertex/Vertex.hpp"
#include "InariKonKon/Graphics/Shader/Shader.hpp"

namespace ikk
{
    template<class VertexType, class IndiciesType = std::uint16_t>
    class VertexInfo
    {
    };

    class ModelBase
    {
    public:
        template<class VertexType, class IndiciesType = std::uint16_t>
        ModelBase(const Shader& fragment, const Shader& vertex, const VertexInfo<VertexType, IndiciesType> info) noexcept;

        virtual ~ModelBase() noexcept = default;

        ModelBase(const ModelBase&) noexcept = default;
        ModelBase(ModelBase&&) noexcept = default;

        ModelBase& operator=(const ModelBase&) noexcept = default;
        ModelBase& operator=(ModelBase&&) noexcept = default;

        virtual const Shader& getFragmentShader() const noexcept final;
        virtual const Shader& getVertexShader() const noexcept final;
    private:
        Shader m_fragment;
        Shader m_vertex;
    };

    template<class VertexType, class IndiciesType>
    ModelBase::ModelBase(const Shader& fragment, const Shader& vertex, const VertexInfo<VertexType, IndiciesType> info) noexcept
        : m_fragment(fragment), m_vertex(vertex)
    {
        
    }

    //TODO:
    //Type safety...
    template<class VertexType, class IndiciesType = std::uint16_t>
    class Model final : public ModelBase
    {
    public:
        Model(const Shader& fragment, const Shader& vertex, const std::vector<VertexType>& vertecies, const std::vector<IndiciesType>& indicies = {});

        ~Model() noexcept = default;

        Model(const Model&) noexcept = default;
        Model(Model&&) noexcept = default;

        Model& operator=(const Model&) noexcept = default;
        Model& operator=(Model&&) noexcept = default;
    private:
        std::vector<VertexType> m_vertecies;
        std::vector<IndiciesType> m_indicies;
    };

    template <class VertexType, class IndiciesType>
    Model<VertexType, IndiciesType>::Model(const Shader& fragment, const Shader& vertex,
                                           const std::vector<VertexType>& vertecies, const std::vector<IndiciesType>& indicies)
        : ModelBase(fragment, vertex, VertexInfo<VertexType, IndiciesType>{}), m_vertecies(vertecies), m_indicies(indicies)
    {
    }
}