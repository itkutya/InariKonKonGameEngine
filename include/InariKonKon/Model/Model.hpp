#pragma once

#include <vector>

#include "InariKonKon/Entitiy/Entity.hpp"
#include "InariKonKon/Model/Vertex.hpp"

namespace ikk
{
    //TODO:
    //Use template for Vertex type & indexBuffer type...
    class Model : public Entity
    {
    public:
        Model(const std::vector<Vertex>& vertecies) noexcept;
        Model(const std::vector<Vertex>& vertecies, const std::vector<std::uint32_t>& indicies) noexcept;

        ~Model() noexcept = default;

        Model(const Model&) noexcept = default;
        Model(Model&&) noexcept = default;

        Model& operator=(const Model&) noexcept = default;
        Model& operator=(Model&&) noexcept = default;

        std::vector<Vertex>& getVertecies() noexcept;
        const std::vector<Vertex>& getVertecies() const noexcept;
    private:
        std::vector<Vertex> m_vertecies;
        std::vector<std::uint32_t> m_indicies;
    };
}