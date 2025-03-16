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
    protected:
        void bind(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint pipelineBindPoint) noexcept;
    private:
        LogicalDevice* m_logicalDevice = nullptr;

        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

        friend class CommandBuffer;
    };
}

//TODO:
//Hash & comp op.
template<>
struct std::hash<ikk::GraphicsPipeline>
{
    std::size_t operator()(const ikk::GraphicsPipeline& graphicsPipeline) const noexcept
    {
        return 0;
    }
};

template <> 
struct std::equal_to<ikk::GraphicsPipeline>
{
    bool operator()(const ikk::GraphicsPipeline& lhs, const ikk::GraphicsPipeline& rhs) const
    {
        return 0 == 0;
    } 
};