#pragma once

#include <cstdint>

namespace ikk
{
    struct Color final
    {
        constexpr Color() noexcept = default;
		inline constexpr Color(const float r, const float g, const float b, const float a = 1.f) noexcept
            : r(r), g(g), b(b), a(a)
        {};
		inline constexpr Color(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = UINT8_MAX) noexcept
			: r(convertU8ToFloat(r)), g(convertU8ToFloat(g)), b(convertU8ToFloat(b)), a(convertU8ToFloat(a))
        {};
        inline constexpr Color(const std::uint32_t color) noexcept
        {
            *this = convertU32ToColor(color);
        };

		constexpr Color(const Color&) noexcept = default;
		constexpr Color(Color&&) noexcept = default;

		constexpr Color& operator=(const Color&) noexcept = default;
		constexpr Color& operator=(Color&&) noexcept = default;

		constexpr ~Color() noexcept = default;

        float r = 0.f, g = 0.f, b = 0.f, a = 1.f;

        static const Color White;
		static const Color Black;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Transparent;

        inline constexpr static const float convertU8ToFloat(const std::uint8_t c) noexcept
        {
            return static_cast<float>(c / UINT8_MAX);
        }

        inline constexpr static const Color convertU32ToColor(const std::uint32_t color) noexcept
        {
            constexpr std::uint32_t redBits{ 0xFF000000 };
            constexpr std::uint32_t greenBits{ 0x00FF0000 };
            constexpr std::uint32_t blueBits{ 0x0000FF00 };
            constexpr std::uint32_t alphaBits{ 0x000000FF };

            Color temp{};
            temp.r = convertU8ToFloat(static_cast<std::uint8_t>((color & redBits) >> 24));
            temp.g = convertU8ToFloat(static_cast<std::uint8_t>((color & greenBits) >> 16));
            temp.b = convertU8ToFloat(static_cast<std::uint8_t>((color & blueBits) >> 8));
            temp.a = convertU8ToFloat(static_cast<std::uint8_t>((color & alphaBits) >> 0));
            return temp;
        }
    };

    inline constexpr Color Color::White{ 1.f, 1.f, 1.f };
	inline constexpr Color Color::Black{ 0.f, 0.f, 0.f };
	inline constexpr Color Color::Red{ 1.f, 0.f, 0.f };
	inline constexpr Color Color::Green{ 0.f, 1.f, 0.f };
	inline constexpr Color Color::Blue{ 0.f, 0.f, 1.f };
	inline constexpr Color Color::Yellow{ 1.f, 1.f, 0.f };
	inline constexpr Color Color::Magenta{ 1.f, 0.f, 1.f };
	inline constexpr Color Color::Cyan{ 0.f, 1.f, 1.f };
	inline constexpr Color Color::Transparent{ 0.f, 0.f, 0.f, 0.f };
}