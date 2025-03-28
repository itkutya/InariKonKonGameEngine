#pragma once

#include <vector>
#include <array>

#include "InariKonKon/Graphics/Vertex/Vertex.hpp"
#include "InariKonKon/Graphics/Shader/Shader.hpp"

namespace ikk
{
    class ModelBase
    {
    public:
        ModelBase(const Shader& fragment, const Shader& vertex, const VertexInfo info) noexcept;

        virtual ~ModelBase() noexcept = default;

        ModelBase(const ModelBase&) noexcept = default;
        ModelBase(ModelBase&&) noexcept = default;

        ModelBase& operator=(const ModelBase&) noexcept = default;
        ModelBase& operator=(ModelBase&&) noexcept = default;

        virtual const Shader& getFragmentShader() const noexcept final;
        virtual const Shader& getVertexShader() const noexcept final;
        virtual const VertexInfo& getVertexInfo() const noexcept final;

        virtual const VertexInfo createVertexInfo() const noexcept = 0;
    private:
        Shader m_fragment;
        Shader m_vertex;

        VertexInfo m_info;
    };

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

        const VertexInfo createVertexInfo() const noexcept override;
    private:
        std::vector<VertexType> m_vertecies;
        std::vector<IndiciesType> m_indicies;
    };

    template <class VertexType, class IndiciesType>
    Model<VertexType, IndiciesType>::Model(const Shader& fragment, const Shader& vertex,
                                           const std::vector<VertexType>& vertecies, const std::vector<IndiciesType>& indicies)
        : ModelBase(fragment, vertex, this->createVertexInfo()), m_vertecies(vertecies), m_indicies(indicies)
    {
    }

    template <class VertexType, class IndiciesType>
    const VertexInfo Model<VertexType, IndiciesType>::createVertexInfo() const noexcept
    {
        VertexInfo temp{};
        temp.binding = 0;
        temp.stride = sizeof(VertexType);

        std::array<VertexAttributes, 2> attributes = {};
        //TODO:
        //Get these from the shader...
        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].offset = offsetof(VertexType, position);
        temp.attributes.emplace_back(attributes[0]);

        attributes[1].binding = 0;
        attributes[1].location = 1;
        attributes[1].offset = offsetof(VertexType, color);
        temp.attributes.emplace_back(attributes[1]);

        return temp;
    }
}