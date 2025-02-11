#include "InariKonKon/Entitiy/Entity.hpp"

namespace ikk
{
    Entity::Entity() noexcept : m_id(Entity::getNextEntityID())
    {
    }

    Entity::Entity(const std::uint32_t id) noexcept : m_id(id)
    {
    }

    const std::uint32_t Entity::getNextEntityID() noexcept
    {
        static std::uint32_t id = 0;
        return ++id;
    }
}