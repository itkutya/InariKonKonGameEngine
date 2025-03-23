#pragma once

#include "InariKonKon/Graphics/Renderer/Vulkan/Renderpass.hpp"

namespace ikk
{
    class Shader;

    class GraphicsPipeline final : public VkType<VkPipeline>
    {
    public:
        GraphicsPipeline(LogicalDevice& logicalDevice, Renderpass& renderpass, const Shader& vertex, const Shader& fragment) noexcept;

        ~GraphicsPipeline() noexcept;

        GraphicsPipeline(const GraphicsPipeline&) noexcept = default;
        GraphicsPipeline(GraphicsPipeline&&) noexcept = default;

        GraphicsPipeline& operator=(const GraphicsPipeline&) noexcept = default;
        GraphicsPipeline& operator=(GraphicsPipeline&&) noexcept = default;

        const std::uint32_t getID() const noexcept;
    protected:
        void bind(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint pipelineBindPoint) noexcept;
    private:
        LogicalDevice* m_logicalDevice = nullptr;

        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

        std::uint32_t m_id = 0;

        static const std::uint32_t createID(const Shader& vertex, const Shader& fragment) noexcept;

        friend class CommandBuffer;
        friend class Vulkan;
    };
}