#include "InariKonKon/Graphics/Renderer/Vulkan/Shader/VkShader.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

#include "InariKonKon/Graphics/Shader/Shader.hpp"

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
        : VkType<VkShaderModule>(createShaderModule(logicalDevice, shader)), m_logicalDevice(&logicalDevice)
    {
        DEBUG_LOG("Vulkan shader created.", Log::INFO, Log::ALL);
    }

    VkShader::~VkShader() noexcept
    {
        vkDestroyShaderModule(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, nullptr);
        DEBUG_LOG("Vulkan shader destroyed.", Log::INFO, Log::ALL);
    }
}