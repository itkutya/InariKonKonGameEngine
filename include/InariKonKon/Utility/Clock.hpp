#pragma once

#include "InariKonKon/Utility/Time.hpp"

namespace ikk
{
	class Clock final
	{
	public:
		Clock() noexcept = default;

		~Clock() noexcept = default;

		Clock(const Clock&) noexcept = default;
		Clock(Clock&&) noexcept = default;

		Clock& operator=(const Clock&) noexcept = default;
		Clock& operator=(Clock&&) noexcept = default;

		[[nodiscard]] const Time getElapsedTime() const noexcept;
		const Time restart() noexcept;
	private:
		std::chrono::steady_clock::time_point m_startPoint = std::chrono::steady_clock::now();
	};
}

template <>
struct std::formatter<ikk::Clock> : std::formatter<ikk::Time>
{
    inline auto format(const ikk::Clock& value, std::format_context& ctx) const
	{
        return std::formatter<ikk::Time>::format(value.getElapsedTime(), ctx);
    }
};