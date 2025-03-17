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

		void start() noexcept;
		void stop() noexcept;
		void wait_for(const Time time) const noexcept;
		
		[[nodiscard]] const Time getElapsedTime() const noexcept;
		[[nodiscard]] const bool isRunning() const noexcept;
		
		const Time restart() noexcept;
		const Time reset() noexcept;
	private:
		std::chrono::steady_clock::time_point m_startPoint{ std::chrono::steady_clock::now() };
		std::chrono::steady_clock::time_point m_stopPoint{};
	};
}