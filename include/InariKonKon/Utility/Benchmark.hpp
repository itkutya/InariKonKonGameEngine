#pragma once

#include "InariKonKon/Utility/Clock.hpp"
#include "InariKonKon/Utility/Logger.hpp"

namespace ikk
{
    class Benchmark
    {
    public:
        inline explicit Benchmark(std::string_view name) noexcept : m_name(name) {};

        inline ~Benchmark() noexcept
        {
            //TODO:
            //Add settings to enable
            //log(std::format("{}: {}", this->m_name, this->m_clock), INFO, ALL);
        };

        Benchmark(const Benchmark&) noexcept = default;
        Benchmark(Benchmark&&) noexcept = default;

        Benchmark& operator=(const Benchmark&) noexcept = default;
        Benchmark& operator=(Benchmark&&) noexcept = default;
    private:
        std::string_view m_name;
        Clock m_clock;
    };
}