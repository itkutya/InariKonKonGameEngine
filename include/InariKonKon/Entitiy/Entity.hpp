#pragma once

#include <cstdint>

#include "InariKonKon/Entitiy/ComponentSystem.hpp"

namespace ikk
{
    class Entity
    {
    public:
        typedef std::uint64_t ID;
        
        Entity() noexcept;
        Entity(const ID id) noexcept;

        virtual ~Entity() noexcept = default;

        Entity(const Entity&) noexcept = default;
        Entity(Entity&&) noexcept = default;

        Entity& operator=(const Entity&) noexcept = default;
        Entity& operator=(Entity&&) noexcept = default;

        template<class T, class... Args>
        void addComponent(Args... args) const noexcept;
        
        template<class T>
        const Component<T>& getComponent() const noexcept;

        template<class T>
        Component<T>& getComponent() noexcept;
    private:
        ID m_id;

        static const ID getNextEntityID() noexcept;
    };

    template <class T, class... Args>
    void Entity::addComponent(Args... args) const noexcept
    {
        ComponentSystem<T>::add(this->m_id, args...);
    }

    template <class T>
    const Component<T>& Entity::getComponent() const noexcept
    {
        return ComponentSystem<T>::get(this->m_id);
    }

    template <class T>
    Component<T>& Entity::getComponent() noexcept
    {
        return ComponentSystem<T>::get(this->m_id);
    }
}