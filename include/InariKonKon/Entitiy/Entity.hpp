#pragma once

#include <cstdint>

namespace ikk
{
    class Entity
    {
    public:
        Entity() noexcept;
        Entity(const std::uint32_t id) noexcept;

        virtual ~Entity() noexcept = default;

        Entity(const Entity&) noexcept = default;
        Entity(Entity&&) noexcept = default;

        Entity& operator=(const Entity&) noexcept = default;
        Entity& operator=(Entity&&) noexcept = default;
    private:
        std::uint32_t m_id;

        static const std::uint32_t getNextEntityID() noexcept;
    };
}