#include "InariKonKon/Graphics/Renderer/Vulkan/Shader/Shader.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

#include "InariKonKon/Graphics/Shader/Shader.hpp"

namespace ikk
{
    VulkanShader::VulkanShader(LogicalDevice& logicalDevice, const Shader& shader) noexcept : m_logicalDevice(&logicalDevice)
    {
        this->m_type = createShaderModule(shader);

        DEBUG_LOG("Vulkan shader created.", Log::INFO, Log::ALL);
    }

    VulkanShader::~VulkanShader() noexcept
    {
        vkDestroyShaderModule(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, nullptr);

        DEBUG_LOG("Vulkan shader destroyed.", Log::INFO, Log::ALL);
    }

    const VkShaderModule VulkanShader::createShaderModule(const Shader& shader) const noexcept
    {
        const std::vector<std::uint32_t>& code = Shader::convertToSPIRV(shader.getShaderCode(), shader.getShaderCodeType());

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size() * sizeof(std::uint32_t);
        createInfo.pCode = code.data();

        VkShaderModule shaderModule;
        VK_CHECK(vkCreateShaderModule(this->m_logicalDevice->getUnderlyingVkType(), &createInfo, nullptr, &shaderModule));

        return shaderModule;
    }
}