#include "InariKonKon/Shader/Shader.hpp"

#include <print>

#include "shaderc/shaderc.hpp"

namespace ikk
{
    Shader::Shader(const Filesystem& code, const decltype(VERTEX) type) noexcept
        : m_code(code.getContents()), m_type(type)
    {
    }

    const std::string& Shader::getShaderCode() const noexcept
    {
        return this->m_code;
    }

    const decltype(Shader::VERTEX)& Shader::getShaderCodeType() const noexcept
    {
        return this->m_type;
    }

    const std::vector<std::uint32_t> Shader::convertToSPIRV(const std::string &shaderCode, const decltype(VERTEX) type) noexcept
    {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        shaderc_shader_kind kind = shaderc_glsl_vertex_shader;
        switch (type)
        {
        case VERTEX:
        kind = shaderc_glsl_vertex_shader;
            break;
        case FRAGMENT:
        kind = shaderc_glsl_fragment_shader;
            break;
        default:
            break;
        }
        shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderCode, kind, "ShaderCode", options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success)
            std::print("{}", module.GetErrorMessage());

        return std::vector<std::uint32_t>{ module.cbegin(), module.cend() };
    }
}