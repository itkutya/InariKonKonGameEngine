#pragma once

#include "InariKonKon/Renderer/Vulkan/Renderpass.hpp"
#include "InariKonKon/Shader/Shader.hpp"

namespace ikk
{
    class GraphicsPipeline : public VkType<VkPipeline>
    {
        class VulkanShader : public VkType<VkShaderModule>
        {
        public:
            VulkanShader(LogicalDevice& logicalDevice, const Shader& shader);

            ~VulkanShader() noexcept;

            VulkanShader(const VulkanShader&) noexcept = default;
            VulkanShader(VulkanShader&&) noexcept = default;

            VulkanShader& operator=(const VulkanShader&) noexcept = default;
            VulkanShader& operator=(VulkanShader&&) noexcept = default;

            [[nodiscard]] const VkShaderModule createShaderModule(const Shader& shader) const;
        private:
            LogicalDevice* m_logicalDevice = nullptr;
        };
    public:
        GraphicsPipeline(LogicalDevice& logicalDevice, Renderpass& renderpass, const Shader& vertex, const Shader& fragment);

        ~GraphicsPipeline() noexcept;

        GraphicsPipeline(const GraphicsPipeline&) noexcept = default;
        GraphicsPipeline(GraphicsPipeline&&) noexcept = default;

        GraphicsPipeline& operator=(const GraphicsPipeline&) noexcept = default;
        GraphicsPipeline& operator=(GraphicsPipeline&&) noexcept = default;
    protected:
        void bind(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint pipelineBindPoint) noexcept;
    private:
        LogicalDevice* m_logicalDevice;

        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

        friend class CommandBuffer;
    };
}