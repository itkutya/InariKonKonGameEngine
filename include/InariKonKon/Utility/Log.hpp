#pragma once

#include <string_view>

#include "fmt/core.h"

#include "InariKonKon/Utility/Utility.hpp"

namespace ikk
{
    class Log final
    {
    public:
        enum LEVEL : std::uint8_t
        {
            INFO = 0, WARNING, ERROR, FATAL
        };

        enum FLAGS : std::uint32_t
        {
            NONE        = 0 << 0,
            DATE        = 1 << 0,
            SEVERITY    = 1 << 1,
            ALL         = NONE | DATE | SEVERITY
        };
        
        Log(const std::string_view msg, const LEVEL level = Log::INFO, const FLAGS flags = Log::ALL) noexcept;

        ~Log() noexcept = default;

		Log(const Log&) noexcept = default;
		Log(Log&&) noexcept = default;

		Log& operator=(const Log&) noexcept = default;
		Log& operator=(Log&&) noexcept = default;
    private:
    };
}

#ifdef IKK_DEBUG
    #define DEBUG_LOG(msg, lvl, flags) ikk::Log(msg, lvl, flags)
#else
    #define DEBUG_LOG(msg, lvl, flags) ((void)0)
#endif