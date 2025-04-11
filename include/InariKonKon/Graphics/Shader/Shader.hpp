#ifndef IKK_SHADER_HPP
#define IKK_SHADER_HPP

#include <vector>

#include "InariKonKon/Utility/Filesystem.hpp"

namespace ikk
{
    //TODO:
    //Hash it...
    class Shader final
    {
    public:
        enum TYPE : std::uint8_t
        {
            VERTEX = 0, FRAGMENT
        };

        explicit Shader(const Filesystem& code, const TYPE type) noexcept;

        ~Shader() noexcept = default;

        Shader(const Shader&) noexcept = default;
        Shader(Shader&&) noexcept = default;

        Shader& operator=(const Shader&) noexcept = default;
        Shader& operator=(Shader&&) noexcept = default;

        const std::string& getShaderCode() const noexcept;
        const TYPE& getShaderCodeType() const noexcept;

        static const std::vector<std::uint32_t> convertToSPIRV(const std::string& shaderCode, const TYPE type);
    private:
        std::string m_code;
        TYPE m_type;
    };
}

#endif