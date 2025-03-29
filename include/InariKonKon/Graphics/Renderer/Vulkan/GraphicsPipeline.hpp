#pragma once

#include "InariKonKon/Graphics/Renderer/Vulkan/Renderpass.hpp"

namespace ikk
{
    class Shader;
    struct VertexInfo;

    class GraphicsPipeline final : public VkType<VkPipeline>
    {
    public:
        GraphicsPipeline(LogicalDevice& logicalDevice, Renderpass& renderpass, const Shader& fragment, const Shader& vertex, const VertexInfo& info) noexcept;

        ~GraphicsPipeline() noexcept;

        GraphicsPipeline(const GraphicsPipeline&) noexcept = default;
        GraphicsPipeline(GraphicsPipeline&&) noexcept = default;

        GraphicsPipeline& operator=(const GraphicsPipeline&) noexcept = default;
        GraphicsPipeline& operator=(GraphicsPipeline&&) noexcept = default;
    protected:
        void bind(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint pipelineBindPoint) noexcept;
    private:
        LogicalDevice* m_logicalDevice = nullptr;

        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

        friend class CommandBuffer;
    };
}