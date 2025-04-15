#ifndef IKK_VULKAN_SHADER_HPP
#define IKK_VULKAN_SHADER_HPP

#include "InariKonKon/Graphics/Renderer/Vulkan/LogicalDevice.hpp"

namespace ikk
{
    class Shader;
    
    class VkShader final : public VkType<VkShaderModule>
    {
    public:
        VkShader(LogicalDevice& logicalDevice, const Shader& shader) noexcept;

        ~VkShader() noexcept;

        VkShader(const VkShader&) noexcept = default;
        VkShader(VkShader&&) noexcept = default;

        VkShader& operator=(const VkShader&) noexcept = default;
        VkShader& operator=(VkShader&&) noexcept = default;
    private:
        LogicalDevice* m_logicalDevice = nullptr;
    };
}

#endif