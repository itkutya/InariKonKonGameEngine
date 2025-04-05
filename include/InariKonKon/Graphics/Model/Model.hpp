#pragma once

#include <vector>
#include <array>

#include "InariKonKon/Graphics/Vertex/Vertex.hpp"
#include "InariKonKon/Graphics/Shader/Shader.hpp"

namespace ikk
{
    template<class VerteciesType, class IndiciesType = void*>
    class Model
    {
    public:
        using VertexType = VerteciesType;
        using IndexType = IndiciesType;

        Model(const Shader& fragment, const Shader& vertex, const std::vector<VerteciesType>& vertecies, const std::vector<IndiciesType>& indicies = {}) noexcept;

        virtual ~Model() noexcept = default;

        Model(const Model&) noexcept = default;
        Model(Model&&) noexcept = default;

        Model& operator=(const Model&) noexcept = default;
        Model& operator=(Model&&) noexcept = default;

        virtual const Shader& getFragmentShader() const noexcept final;
        virtual const Shader& getVertexShader() const noexcept final;
        virtual const VertexInfo& getVertexInfo() const noexcept final;

        virtual const std::vector<VerteciesType>& getVertecies() const noexcept final;
        virtual const std::vector<IndiciesType>& getIndicies() const noexcept final;
    protected:
        virtual const VertexInfo createVertexInfo() const noexcept;
    private:
        Shader m_fragment;
        Shader m_vertex;
        
        VertexInfo m_info;
        
        std::vector<VerteciesType> m_vertecies;
        std::vector<IndiciesType> m_indicies;
    };

    template <class VerteciesType, class IndiciesType>
    Model<VerteciesType, IndiciesType>::Model(const Shader& fragment, const Shader& vertex,
                                           const std::vector<VerteciesType>& vertecies, const std::vector<IndiciesType>& indicies) noexcept
        : m_fragment(fragment), m_vertex(vertex), m_info(this->createVertexInfo()), m_vertecies(vertecies), m_indicies(indicies)
    {
    }

    template <class VerteciesType, class IndiciesType>
    const Shader& Model<VerteciesType, IndiciesType>::getFragmentShader() const noexcept
    {
        return this->m_fragment;
    }
    
    template <class VerteciesType, class IndiciesType>
    const Shader& Model<VerteciesType, IndiciesType>::getVertexShader() const noexcept
    {
        return this->m_vertex;
    }

    template <class VerteciesType, class IndiciesType>
    const VertexInfo& Model<VerteciesType, IndiciesType>::getVertexInfo() const noexcept
    {
        return this->m_info;
    }

    template <class VerteciesType, class IndiciesType>
    const std::vector<VerteciesType>& Model<VerteciesType, IndiciesType>::getVertecies() const noexcept
    {
        return this->m_vertecies;
    }

    template <class VerteciesType, class IndiciesType>
    const std::vector<IndiciesType>& Model<VerteciesType, IndiciesType>::getIndicies() const noexcept
    {
        return this->m_indicies;
    }

    template <class VerteciesType, class IndiciesType>
    const VertexInfo Model<VerteciesType, IndiciesType>::createVertexInfo() const noexcept
    {
        VertexInfo temp{};
        temp.binding = 0;
        temp.stride = sizeof(VerteciesType);
        temp.inputRate = VertexInfo::InputRate::Per_Vertex;

        std::array<VertexAttributes, 2> attributes = {};
        //TODO:
        //Get these from the shader...
        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].offset = offsetof(VerteciesType, position);
        attributes[0].format = VertexAttributes::Format::Vec2;
        temp.attributes.emplace_back(attributes[0]);

        attributes[1].binding = 0;
        attributes[1].location = 1;
        attributes[1].offset = offsetof(VerteciesType, color);
        attributes[1].format = VertexAttributes::Format::Vec4;
        temp.attributes.emplace_back(attributes[1]);

        return temp;
    }
}