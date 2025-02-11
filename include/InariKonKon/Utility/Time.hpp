#pragma once

#include <assert.h>
#include <cstdint>
#include <chrono>
#include <format>

namespace ikk
{
	class Time final
	{
	public:
		constexpr Time() noexcept = default;
		template <typename Rep, typename Period>
		explicit constexpr Time(const std::chrono::duration<Rep, Period>& duration) noexcept;
		explicit constexpr Time(const float amount) noexcept;
		explicit constexpr Time(const std::uint32_t amount) noexcept;
		explicit constexpr Time(const std::uint64_t amount) noexcept;

		constexpr ~Time() noexcept = default;

		constexpr Time(const Time&) noexcept = default;
		constexpr Time(Time&&) noexcept = default;

		constexpr Time& operator=(const Time&) noexcept = default;
		constexpr Time& operator=(Time&&) noexcept = default;

		[[nodiscard]] constexpr float asSeconds() const noexcept;
		[[nodiscard]] constexpr std::int32_t asMilliseconds() const noexcept;
		[[nodiscard]] constexpr std::int64_t asMicroseconds() const noexcept;

		[[nodiscard]] constexpr std::chrono::microseconds toDuration() const noexcept;

		[[nodiscard]] const auto operator<=>(const Time&) const = default;
	private:
		std::chrono::microseconds m_value{};
	};

	template <typename Rep, typename Period>
	constexpr Time::Time(const std::chrono::duration<Rep, Period>& duration) noexcept
        : m_value(std::chrono::duration_cast<std::chrono::microseconds>(duration))
	{
	}

	constexpr Time::Time(const float amount) noexcept
        : m_value(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<float>(amount)))
	{
		assert((amount >= 0.f) && "Time amount cannot be negative number!");
	}

	constexpr Time::Time(const std::uint32_t amount) noexcept
        : m_value(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(amount)))
	{
	}

	constexpr Time::Time(const std::uint64_t amount) noexcept
        : m_value(std::chrono::microseconds(amount))
	{
	}

	constexpr float Time::asSeconds() const noexcept
	{
		return static_cast<float>(this->m_value.count() / 1000000.f);
	}

	constexpr std::int32_t Time::asMilliseconds() const noexcept
	{
		return static_cast<std::int32_t>(this->m_value.count() / 1000u);
	}

	constexpr std::int64_t Time::asMicroseconds() const noexcept
	{
		return static_cast<std::int64_t>(this->m_value.count());
	}

	constexpr std::chrono::microseconds Time::toDuration() const noexcept
	{
		return this->m_value;
	}

	[[nodiscard]] inline constexpr Time seconds(const float amount) noexcept
	{
		return Time(amount);
	}

	[[nodiscard]] inline constexpr Time milliseconds(const std::uint32_t amount) noexcept
	{
		return Time(amount);
	}

	[[nodiscard]] inline constexpr Time microseconds(const std::uint64_t amount) noexcept
	{
		return Time(amount);
	}
}

template<>
struct std::formatter<ikk::Time> : std::formatter<std::chrono::microseconds>
{
    inline auto format(const ikk::Time& value, std::format_context& ctx) const
	{
        return std::formatter<std::chrono::microseconds>::format(value.toDuration(), ctx);
    }
};