#include "InariKonKon/Entitiy/EntitySystem.hpp"

#include "InariKonKon/Entitiy/Entity.hpp"

namespace ikk
{
    const std::vector<Entity*>& EntitySystem::getEntities() const noexcept
    {
        return EntitySystem::m_entities;
    }
    
    void EntitySystem::addEntityToList(Entity& entity) noexcept
    {
        EntitySystem::m_entities.emplace_back(&entity);
    }
}