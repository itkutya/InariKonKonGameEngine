#include "InariKonKon/Graphics/Renderer/Vulkan/Shader/VkShader.hpp"

#include <unordered_map>
#include <string_view>
#include <exception>
#include <ranges>

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
    [[nodiscard]] VkShaderModule createShaderModule(LogicalDevice& logicalDevice, const Shader& shader) noexcept
    {
        const std::vector<std::uint32_t>& code = Shader::convertToSPIRV(shader.getShaderCode(), shader.getShaderCodeType());

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size() * sizeof(std::uint32_t);
        createInfo.pCode = code.data();

        VkShaderModule shaderModule;
        VK_CHECK(vkCreateShaderModule(logicalDevice.getUnderlyingVkType(), &createInfo, nullptr, &shaderModule));
        return shaderModule;
    }

    VkShader::VkShader(LogicalDevice& logicalDevice, const Shader& shader) noexcept
        : VkType<VkShaderModule>(createShaderModule(logicalDevice, shader)), Shader(shader), m_logicalDevice(&logicalDevice)
    {
        DEBUG_LOG("Vulkan shader created.", Log::INFO, Log::ALL);
    }

    VkShader::~VkShader() noexcept
    {
        vkDestroyShaderModule(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, nullptr);
        DEBUG_LOG("Vulkan shader destroyed.", Log::INFO, Log::ALL);
    }

    const VkShader::VertexInfo VkShader::createVertexInfo(const std::uint32_t binding, const VertexInfo::InputRate inputRate) const
    {
        if (this->getShaderCodeType() != Shader::TYPE::VERTEX)
            throw std::runtime_error("Unsupported shader type");

        std::vector<VertexAttributes::Format> types{};
        std::uint32_t total_size = 0;

        for (const auto line : std::views::split(this->getShaderCode(), '\n'))
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