#ifndef IKK_VULKAN_SHADER_HPP
#define IKK_VULKAN_SHADER_HPP

#include "InariKonKon/Graphics/Renderer/Vulkan/LogicalDevice.hpp"

namespace ikk
{
    class Shader;
    
    class VulkanShader final : public VkType<VkShaderModule>
    {
    public:
        VulkanShader(LogicalDevice& logicalDevice, const Shader& shader) noexcept;

        ~VulkanShader() noexcept;

        VulkanShader(const VulkanShader&) noexcept = default;
        VulkanShader(VulkanShader&&) noexcept = default;

        VulkanShader& operator=(const VulkanShader&) noexcept = default;
        VulkanShader& operator=(VulkanShader&&) noexcept = default;
    private:
        LogicalDevice* m_logicalDevice = nullptr;
        
        [[nodiscard]] const VkShaderModule createShaderModule(const Shader& shader) const noexcept;
    };
}

#endif