#pragma once

#include "InariKonKon/Utility/Benchmark.hpp"

#ifdef NDEBUG
    #define IKK_RELEASE 1

    #define BENCHMARK_BEGIN(name)           ((void)0)
    #define BENCHMARK_END()                 ((void)0)
    #define BENCHMARK_BLOCK(name)           ((void)0)

    #define DEBUG_LOG(msg, level, flags)    ((void)0)
#else
    #define IKK_DEBUG 1

    #define BENCHMARK_BEGIN(name)   { [[maybe_unused]] const ikk::Benchmark _{ std::string_view(name) }
    #define BENCHMARK_END()         } ((void)0)
    #define BENCHMARK_BLOCK(name)   [[maybe_unused]] const ikk::Benchmark _{ std::string_view(name) }

    #define DEBUG_LOG(msg, level, flags)    ikk::log(msg, level, flags)
#endif