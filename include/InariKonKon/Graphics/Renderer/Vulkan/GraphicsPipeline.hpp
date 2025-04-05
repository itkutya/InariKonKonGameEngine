#pragma once

#include "InariKonKon/Graphics/Renderer/Vulkan/Renderpass.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan/Shader/Shader.hpp"
#include "InariKonKon/Graphics/Model/Model.hpp"

namespace ikk
{
    class GraphicsPipeline final : public VkType<VkPipeline>
    {
    public:
        GraphicsPipeline(LogicalDevice& logicalDevice) noexcept;

        ~GraphicsPipeline() noexcept;

        GraphicsPipeline(const GraphicsPipeline&) noexcept = default;
        GraphicsPipeline(GraphicsPipeline&&) noexcept = default;

        GraphicsPipeline& operator=(const GraphicsPipeline&) noexcept = default;
        GraphicsPipeline& operator=(GraphicsPipeline&&) noexcept = default;
        
        void create(Renderpass& renderpass, const Shader& fragment, const Shader& vertex, const VertexInfo& info) noexcept;
    protected:
        void bind(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint pipelineBindPoint) noexcept;
    private:
        LogicalDevice* m_logicalDevice = nullptr;

        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

        friend class CommandBuffer;
    };
}