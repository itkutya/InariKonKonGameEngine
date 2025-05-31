#include "InariKonKon/Utility/Clock.hpp"

#include <thread>

namespace ikk
{
    void Clock::start() noexcept
    {
        if (this->isRunning() == false)
        {
            this->m_startPoint += std::chrono::steady_clock::now() - this->m_stopPoint;
            this->m_stopPoint = {};
        }
    }

    void Clock::stop() noexcept
    {
        if (this->isRunning())
            this->m_stopPoint = std::chrono::steady_clock::now();
    }

    void Clock::wait_for(const Time time) const noexcept
    {
        std::this_thread::sleep_for(time.toDuration());
    }

    const Time Clock::getElapsedTime() const noexcept
    {
        if (this->isRunning())
            return Time(std::chrono::steady_clock::now() - this->m_startPoint);
        return Time(this->m_stopPoint - this->m_startPoint);
    }

    const bool Clock::isRunning() const noexcept
    {
        return this->m_stopPoint == std::chrono::steady_clock::time_point();
    }

    const Time Clock::restart() noexcept
    {
        const Time elapsed = this->getElapsedTime();
		this->m_startPoint = std::chrono::steady_clock::now();
        this->m_stopPoint = {};
		return elapsed;
    }

    const Time Clock::reset() noexcept
    {
        const Time elapsed = this->getElapsedTime();
		this->m_startPoint = std::chrono::steady_clock::now();
        this->m_stopPoint = this->m_startPoint;
		return elapsed;
    }
}