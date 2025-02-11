#pragma once

#include <cstdint>
#include <type_traits>
#include <variant>

#include <glm/vec2.hpp>

namespace ikk
{
    class Event final
    {
    public:
        struct WindowClosed
        {
        };

        struct WindowResized
        {
            std::uint32_t width;
            std::uint32_t height;
        };

        struct WindowFramebufferResized
        {
            std::uint32_t width;
            std::uint32_t height;
        };

        struct FocusLost
        {
        };

        struct FocusGained
        {
        };

        struct TextEntered
        {
            char32_t unicode{};
        };

        struct KeyPressed
        {
            //Keyboard::Key      code{};
            //Keyboard::Scancode scancode{};
            bool               alt{};
            bool               control{};
            bool               shift{};
            bool               system{};
        };

        struct KeyReleased
        {
            //Keyboard::Key      code{};
            //Keyboard::Scancode scancode{};
            bool               alt{};
            bool               control{};
            bool               shift{};
            bool               system{};
        };

        struct MouseWheelScrolled
        {
            //Mouse::Wheel wheel{};
            float delta{};
            glm::ivec2 position;
        };

        struct MouseButtonPressed
        {
            //Mouse::Button button{};
            glm::ivec2      position;
        };

        struct MouseButtonReleased
        {
            //Mouse::Button button{};
            glm::ivec2      position;
        };

        struct MouseMoved
        {
            glm::ivec2 position;
        };

        struct MouseMovedRaw
        {
            glm::ivec2 delta;
        };

        struct MouseEnteredWindow
        {
        };

        struct MouseLeftWindow
        {
        };

        struct JoystickButtonPressed
        {
            std::uint32_t joystickId{};
            std::uint32_t button{};
        };

        struct JoystickButtonReleased
        {
            std::uint32_t joystickId{};
            std::uint32_t button{};
        };

        struct JoystickMoved
        {
            std::uint32_t   joystickId{};
            //Joystick::Axis axis{};
            float          position{};
        };

        struct JoystickConnected
        {
            std::uint32_t joystickId{};
        };

        struct JoystickDisconnected
        {
            std::uint32_t joystickId{};
        };

        template <class T>
        Event(const T& type) noexcept;

        template <class T>
        [[nodiscard]] const bool is() const noexcept;

        template <class T>
        [[nodiscard]] T& get() noexcept;

        template <class T>
        [[nodiscard]] const T& get() const noexcept;

        template <class T>
        [[nodiscard]] T* getIf() noexcept;

        template <class T>
        [[nodiscard]] const T* getIf() const noexcept;
    private:
        std::variant<
                WindowClosed,
                WindowResized,
                WindowFramebufferResized,
                FocusLost,
                FocusGained,
                TextEntered,
                KeyPressed,
                KeyReleased,
                MouseWheelScrolled,
                MouseButtonPressed,
                MouseButtonReleased,
                MouseMoved,
                MouseMovedRaw,
                MouseEnteredWindow,
                MouseLeftWindow,
                JoystickButtonPressed,
                JoystickButtonReleased,
                JoystickMoved,
                JoystickConnected,
                JoystickDisconnected>
            m_data;

        template <class T, class... Ts>
        [[nodiscard]] inline static constexpr const bool isInParameterPack(const std::variant<Ts...>*) noexcept
        {
            return std::disjunction_v<std::is_same<T, Ts>...>;
        }

        template <class T>
        inline static constexpr bool isEventSubtype = isInParameterPack<T>(decltype (&m_data)(nullptr));
    };

    template <class T>
    Event::Event(const T& type) noexcept
    {
        static_assert(isEventSubtype<T>, "T must be a subtype of ikk::Event");
        if constexpr (isEventSubtype<T>)
            this->m_data = type;
    }

    template <typename T>
    const bool Event::is() const noexcept
    {
        static_assert(isEventSubtype<T>, "T must be a subtype of ikk::Event");
        if constexpr (isEventSubtype<T>)
            return std::holds_alternative<T>(this->m_data);
    }

    template <class T>
    T& Event::get() noexcept
    {
        static_assert(isEventSubtype<T>, "T must be a subtype of ikk::Event");
        if constexpr (isEventSubtype<T>)
            return std::get<T>(this->m_data);
    }

    template <class T>
    const T& Event::get() const noexcept
    {
        static_assert(isEventSubtype<T>, "T must be a subtype of ikk::Event");
        if constexpr (isEventSubtype<T>)
            return std::get<T>(this->m_data);
    }

    template <class T>
    T* Event::getIf() noexcept
    {
        static_assert(isEventSubtype<T>, "T must be a subtype of ikk::Event");
        if constexpr (isEventSubtype<T>)
            return std::get_if<T>(&this->m_data);
    }

    template <class T>
    const T* Event::getIf() const noexcept
    {
        static_assert(isEventSubtype<T>, "T must be a subtype of ikk::Event");
        if constexpr (isEventSubtype<T>)
            return std::get_if<T>(&this->m_data);
    }
}