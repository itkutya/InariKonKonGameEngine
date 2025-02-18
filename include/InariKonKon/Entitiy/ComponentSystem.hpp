#pragma once

#include <cstdint>
#include <unordered_map>

#include "InariKonKon/Entitiy/Component.hpp"

namespace ikk
{
    //TODO:
    //Make this class into a singleton...
    template<class T>
    class ComponentSystem final
    {
    public:
        ComponentSystem() noexcept = default;

        ~ComponentSystem() noexcept = default;

        ComponentSystem(const ComponentSystem&) noexcept = default;
        ComponentSystem(ComponentSystem&&) noexcept = default;

        ComponentSystem& operator=(const ComponentSystem&) noexcept = default;
        ComponentSystem& operator=(ComponentSystem&&) noexcept = default;  
        
        template<class... Args>
        static void add(const std::uint64_t entityID, Args... args) noexcept;
        
        static Component<T>& get(const std::uint64_t entityID) noexcept;
    private:
        inline static std::unordered_map<std::uint64_t, Component<T>> m_components;
    };

    template <class T>
    template <class... Args>
    void ComponentSystem<T>::add(const std::uint64_t entityID, Args... args) noexcept
    {
        ComponentSystem::m_components.emplace(std::make_pair(entityID, Component<T>{ args... }));
    }

    //TODO:
    //Should throw in debug mode otherwise no
    template <class T>
    Component<T> &ComponentSystem<T>::get(const std::uint64_t entityID) noexcept
    {
        return m_components.at(entityID);
    }
}