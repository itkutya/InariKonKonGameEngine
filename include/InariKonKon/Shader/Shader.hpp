#pragma once

#include <cstdint>
#include <vector>

#include "InariKonKon/Filesystem/Filesystem.hpp"

namespace ikk
{
    class Shader
    {
    public:
        enum : std::uint8_t
        {
            VERTEX = 0, FRAGMENT
        };

        explicit Shader(const Filesystem& code, const decltype(VERTEX) type) noexcept;

        ~Shader() noexcept = default;

        Shader(const Shader&) noexcept = default;
        Shader(Shader&&) noexcept = default;

        Shader& operator=(const Shader&) noexcept = default;
        Shader& operator=(Shader&&) noexcept = default;

        const std::string& getShaderCode() const noexcept;
        const decltype(VERTEX)& getShaderCodeType() const noexcept;

        static const std::vector<std::uint32_t> convertToSPIRV(const std::string& shaderCode, const decltype(VERTEX) type) noexcept;
    private:
        std::string m_code;
        decltype(VERTEX) m_type;
    };
}