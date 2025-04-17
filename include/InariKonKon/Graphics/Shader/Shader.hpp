#ifndef IKK_SHADER_HPP
#define IKK_SHADER_HPP

#include <functional>

#include "InariKonKon/Utility/Filesystem.hpp"

namespace ikk
{
    class Shader
    {
    public:
        enum TYPE : std::uint8_t
        {
            VERTEX = 0, FRAGMENT
        };

        explicit Shader(const Filesystem& code, const TYPE type) noexcept;

        virtual ~Shader() noexcept = default;

        Shader(const Shader&) noexcept = default;
        Shader(Shader&&) noexcept = default;

        Shader& operator=(const Shader&) noexcept = default;
        Shader& operator=(Shader&&) noexcept = default;

        virtual const std::string& getShaderCode() const noexcept final;
        virtual const TYPE& getShaderCodeType() const noexcept final;

        static const std::vector<std::uint32_t> convertToSPIRV(const std::string& shaderCode, const TYPE type);
    private:
        std::string m_code;
        TYPE m_shaderType;
    };
}

template<>
struct std::hash<ikk::Shader>
{
    inline const std::size_t operator()(const ikk::Shader& shader) const noexcept
    {
        return std::hash<std::string>{}(shader.getShaderCode());
    }
};

template<>
struct std::equal_to<ikk::Shader>
{
    inline const bool operator()(const ikk::Shader& lhs, const ikk::Shader& rhs) const noexcept
    {
        return std::hash<std::string>{}(lhs.getShaderCode()) == std::hash<std::string>{}(rhs.getShaderCode());
    }
};

#endif