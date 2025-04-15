#ifndef IKK_SHADER_HPP
#define IKK_SHADER_HPP

#include <functional>
#include <vector>

#include "InariKonKon/Utility/Filesystem.hpp"

namespace ikk
{
    class Shader final
    {
    public:
        enum TYPE : std::uint8_t
        {
            VERTEX = 0, FRAGMENT
        };

        struct VertexAttributes
        {
            enum struct Format
            {
                Float, Vec2, Vec3, Vec4
            };

            std::uint32_t binding = 0;
            std::uint32_t location = 0;
            Format format = Format::Float;
            std::uint32_t offset = 0;
        };

        struct VertexInfo
        {
            enum struct InputRate
            {
                Per_Vertex, Per_Instance
            };

            std::uint32_t binding = 0;
            std::uint32_t stride = 0;
            InputRate inputRate = InputRate::Per_Vertex;
            std::vector<VertexAttributes> attributes{};
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
        
        const VertexInfo createVertexInfo(const std::uint32_t binding = 0, const VertexInfo::InputRate inputRate = VertexInfo::InputRate::Per_Vertex) const;
    private:
        std::string m_code;
        TYPE m_type;
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