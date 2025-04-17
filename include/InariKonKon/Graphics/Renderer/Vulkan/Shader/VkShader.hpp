#ifndef IKK_VULKAN_SHADER_HPP
#define IKK_VULKAN_SHADER_HPP

#include "InariKonKon/Graphics/Renderer/Vulkan/LogicalDevice.hpp"

#include "InariKonKon/Graphics/Shader/Shader.hpp"

namespace ikk
{
    class VkShader final : public VkType<VkShaderModule>, public Shader
    {
    public:
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
        
        VkShader(LogicalDevice& logicalDevice, const Shader& shader) noexcept;

        ~VkShader() noexcept;

        VkShader(const VkShader&) noexcept = default;
        VkShader(VkShader&&) noexcept = default;

        VkShader& operator=(const VkShader&) noexcept = default;
        VkShader& operator=(VkShader&&) noexcept = default;

        const VertexInfo createVertexInfo(const std::uint32_t binding = 0, const VertexInfo::InputRate inputRate = VertexInfo::InputRate::Per_Vertex) const;
    private:
        LogicalDevice* m_logicalDevice = nullptr;
    };
}

#endif