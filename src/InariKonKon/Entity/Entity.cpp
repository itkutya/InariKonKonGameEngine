#include "InariKonKon/Entitiy/Entity.hpp"

#include "InariKonKon/Entitiy/EntitySystem.hpp"

namespace ikk
{
    Entity::Entity() noexcept : m_id(Entity::getNextEntityID())
    {
        EntitySystem::addEntityToList(*this);
    }

    Entity::Entity(const ID id) noexcept : m_id(id)
    {
        EntitySystem::addEntityToList(*this);
    }

    const Entity::ID Entity::getNextEntityID() noexcept
    {
        static ID id = 0;
        return ++id;
    }
}