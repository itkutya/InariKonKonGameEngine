#pragma once

#include <type_traits>
#include <cstdint>

namespace ikk
{
    template<class T>
    struct Rect
    {
        static_assert(std::is_arithmetic_v<T> == true);
        T x = static_cast<T>(0), y = static_cast<T>(0), width = static_cast<T>(0), height = static_cast<T>(0);
    };

    using fRect = Rect<float>;
    using iRect = Rect<std::int32_t>;
    using u32Rect = Rect<std::uint32_t>;
}