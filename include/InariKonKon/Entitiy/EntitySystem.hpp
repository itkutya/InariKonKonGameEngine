#pragma once

#include <vector>

namespace ikk
{
    class Entity;

    class EntitySystem final
    {
    public:
        EntitySystem() noexcept = default;

        ~EntitySystem() noexcept = default;

        EntitySystem(const EntitySystem&) noexcept = default;
        EntitySystem(EntitySystem&&) noexcept = default;

        EntitySystem& operator=(const EntitySystem&) noexcept = default;
        EntitySystem& operator=(EntitySystem&&) noexcept = default;

        const std::vector<Entity*>& getEntities() const noexcept;
    private:
        inline static std::vector<Entity*> m_entities;
        
        static void addEntityToList(Entity& entity) noexcept;

        friend class Entity;
    };
}