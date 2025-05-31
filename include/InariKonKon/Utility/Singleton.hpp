#ifndef IKK_SINGLETON_HPP
#define IKK_SINGLETON_HPP

#include "InariKonKon/Utility/NonCopyable.hpp"

namespace ikk
{
    template<class T>
    class Singleton : public NonCopyable
    {
    public:
        Singleton(Singleton&&) noexcept = delete;
        Singleton& operator=(Singleton&&) noexcept = delete;
        
        virtual ~Singleton() noexcept = default;

        [[nodiscard]] static T& getInstance() noexcept;
    protected:
        Singleton() noexcept = default;
    };

    template<class T>
    T& Singleton<T>::getInstance() noexcept
    {
        static T instance;
        return instance;
    }
}

#endif