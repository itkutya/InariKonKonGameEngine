#include "InariKonKon/Graphics/Shader/Shader.hpp"

#include <unordered_map>
#include <string_view>
#include <exception>
#include <ranges>

#include "shaderc/shaderc.hpp"
#include "fmt/core.h"

namespace ikk
{
    Shader::Shader(const Filesystem& code, const TYPE type) noexcept : m_code(code.getContents()), m_type(type)
    {
    }

    const std::string& Shader::getShaderCode() const noexcept
    {
        return this->m_code;
    }

    const Shader::TYPE& Shader::getShaderCodeType() const noexcept
    {
        return this->m_type;
    }

    const std::vector<std::uint32_t> Shader::convertToSPIRV(const std::string& shaderCode, const TYPE type)
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
            throw std::runtime_error("Unsupported shader type");
            return {};
        }
        shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderCode, kind, "ShaderCode", options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success)
            fmt::print("{}", module.GetErrorMessage());

        return std::vector<std::uint32_t>{ module.cbegin(), module.cend() };
    }

    const Shader::VertexInfo Shader::createVertexInfo(const std::uint32_t binding, const VertexInfo::InputRate inputRate) const
    {
        if (this->m_type != Shader::TYPE::VERTEX)
            throw std::runtime_error("Unsupported shader type");

        std::vector<VertexAttributes::Format> types{};
        std::uint32_t total_size = 0;

        for (const auto line : std::views::split(this->m_code, '\n'))
        {
            const std::string line_str{ line.begin(), line.end() };

            if (line_str.contains("layout") == false)
                continue;
                
            if (line_str.contains(" in ") == false)
                continue;

            const auto start_of_var = line_str.find(" in ") + 4;
            const auto end_of_var = line_str.find_last_of(' ') - 1;
            const std::string var = line_str.substr(start_of_var, (end_of_var) - (start_of_var) + 1);

            static std::unordered_map<std::string_view, VertexAttributes::Format> formatConverter =
            { {
                { "float", VertexAttributes::Format::Float },
                { "vec2",  VertexAttributes::Format::Vec2  },
                { "vec3",  VertexAttributes::Format::Vec3  },
                { "vec4",  VertexAttributes::Format::Vec4  }
            } };

            if (formatConverter.contains(var) == false)
                throw std::runtime_error("Unsupported shader type");

            types.emplace_back(formatConverter.at(var));
            switch (types.back())
            {
            case VertexAttributes::Format::Vec4:
                total_size += sizeof(float);
                [[fallthrough]];
            case VertexAttributes::Format::Vec3:
                total_size += sizeof(float);
                [[fallthrough]];
            case VertexAttributes::Format::Vec2:
                total_size += sizeof(float);
                [[fallthrough]];
            case VertexAttributes::Format::Float:
                total_size += sizeof(float);
                break;
            }
        }
            
        VertexInfo temp{};
        temp.binding = binding;
        temp.stride = total_size;
        temp.inputRate = inputRate;

        for (std::size_t i = 0; i < types.size(); ++i)
        {
            VertexAttributes vertexAttribute{};
            //TODO:
            //Save it from the shader...
            static std::size_t temp_location = 0;
            vertexAttribute.binding = binding;
            vertexAttribute.location = temp_location++;
            //TODO:
            //Calculate this somehow...
            /*
            https://vkguide.dev/docs/chapter-4/descriptors_code_more/
            
            size_t VulkanEngine::pad_uniform_buffer_size(size_t originalSize)
            {
                // Calculate required alignment based on minimum device offset alignment
                size_t minUboAlignment = _gpuProperties.limits.minUniformBufferOffsetAlignment;
                size_t alignedSize = originalSize;
                if (minUboAlignment > 0) {
                    alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
                }
                return alignedSize;
            }
            */
            static std::size_t temp_offset = 0;
            if (i == 0)
                vertexAttribute.offset = 0;
            else
                vertexAttribute.offset = 8;

            vertexAttribute.format = types.at(i);
            temp.attributes.emplace_back(vertexAttribute);
        }

        return temp;
    }
}