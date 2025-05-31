#include "InariKonKon/Utility/Log.hpp"

#include <string>
#include <chrono>

#include "fmt/core.h"
#include "fmt/chrono.h"

namespace ikk
{
    Log::Log(const std::string_view msg, const LEVEL level, const FLAGS flags) noexcept
    {
        std::string message{};

        if (flags & SEVERITY)
            switch (level)
            {
            case INFO:
                message += "[INFO]: ";
                break;
            case WARNING:
                message += "[WARNING]: ";
                break;
            case ERROR:
                message += "[ERROR]: ";
                break;
            case FATAL:
                message += "[FATAL]: ";
                break;
            default:
                message += "[]: ";
                break;
            }

        message += msg.data();

        if (flags & DATE)
            message += fmt::format(" {}", std::chrono::system_clock::now()).c_str();

        fmt::println("{}", message);
    }
}