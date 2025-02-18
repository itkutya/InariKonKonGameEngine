#pragma once

namespace ikk
{
    template<class T>
    class Component final
    {
    public:
        template<class... Args>
        Component(Args... args) noexcept;

        ~Component() noexcept = default;

        Component(const Component&) noexcept = default;
        Component(Component&&) noexcept = default;

        Component& operator=(const Component&) noexcept = default;
        Component& operator=(Component&&) noexcept = default;

        const T& get() const noexcept;
        T& get() noexcept;
    private:
        T m_type;
    };

    template <class T>
    template <class... Args>
    Component<T>::Component(Args... args) noexcept : m_type(args...)
    {
    }
    
    template <class T>
    const T& Component<T>::get() const noexcept
    {
        return this->m_type;
    }
    
    template <class T>
    T& Component<T>::get() noexcept
    {
        return this->m_type;
    }
}