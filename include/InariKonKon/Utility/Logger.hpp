#pragma once

#include <string_view>
#include <cstdint>
#include <chrono>
#include <print>

namespace ikk
{
    //TODO:
    //Set printing min severity level or smthg...

    enum : std::uint8_t
    {
        INFO = 0, WARNING, ERROR, FATAL
    };

    enum : std::uint32_t
    {
        NONE        = 0 << 0,
        DATE        = 1 << 0,
        SEVERITY    = 1 << 1,
        ALL         = NONE | DATE | SEVERITY
    };

    static void log(std::string_view msg, const decltype(INFO) level = INFO, const decltype(ALL) flags = ALL) noexcept
    {
        if (flags & SEVERITY)
            switch (level)
            {
            case INFO:
                std::print("[INFO]: ");
                break;
            case WARNING:
                std::print("[WARNING]: ");
                break;
            case ERROR:
                std::print("[ERROR]: ");
                break;
            case FATAL:
                std::print("[FATAL]: ");
                break;
            default:
                std::print("[]: ");
                break;
            }

        std::print("{} ", msg);

        if (flags & DATE)
            std::print("{}", std::chrono::system_clock::now());

        std::print("\n");
    }
}