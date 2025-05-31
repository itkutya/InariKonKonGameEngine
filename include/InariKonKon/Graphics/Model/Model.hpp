#ifndef IKK_MODEL_HPP
#define IKK_MODEL_HPP

#include "InariKonKon/Graphics/Shader/Shader.hpp"
#include "InariKonKon/Graphics/Vertex/Vertex.hpp"

namespace ikk
{
    template<class VerteciesType, class IndiciesType = void*>
    class Model
    {
    public:
        Model(const Shader& fragment, const Shader& vertex, const std::vector<VerteciesType>& vertecies, const std::vector<IndiciesType>& indicies = {}) noexcept;

        virtual ~Model() noexcept = default;

        Model(const Model&) noexcept = default;
        Model(Model&&) noexcept = default;

        Model& operator=(const Model&) noexcept = default;
        Model& operator=(Model&&) noexcept = default;

        virtual const Shader& getFragmentShader() const noexcept final;
        virtual const Shader& getVertexShader() const noexcept final;

        virtual const std::vector<VerteciesType>& getVertecies() const noexcept final;
        virtual const std::vector<IndiciesType>& getIndicies() const noexcept final;
    private:
        Shader m_fragment;
        Shader m_vertex;
        
        std::vector<VerteciesType> m_vertecies;
        std::vector<IndiciesType> m_indicies;
    };

    template<class VerteciesType, class IndiciesType>
    Model<VerteciesType, IndiciesType>::Model(const Shader& fragment, const Shader& vertex,
                            const std::vector<VerteciesType>& vertecies, const std::vector<IndiciesType>& indicies) noexcept
        : m_fragment(fragment), m_vertex(vertex), m_vertecies(vertecies), m_indicies(indicies)
    {
    }

    template<class VerteciesType, class IndiciesType>
    const Shader& Model<VerteciesType, IndiciesType>::getFragmentShader() const noexcept
    {
        return this->m_fragment;
    }
    
    template<class VerteciesType, class IndiciesType>
    const Shader& Model<VerteciesType, IndiciesType>::getVertexShader() const noexcept
    {
        return this->m_vertex;
    }

    template<class VerteciesType, class IndiciesType>
    const std::vector<VerteciesType>& Model<VerteciesType, IndiciesType>::getVertecies() const noexcept
    {
        return this->m_vertecies;
    }

    template<class VerteciesType, class IndiciesType>
    const std::vector<IndiciesType>& Model<VerteciesType, IndiciesType>::getIndicies() const noexcept
    {
        return this->m_indicies;
    }
}

#endif