#pragma once

#include "InariKonKon/Graphics/Renderer/Vulkan/Renderpass.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan/Shader/Shader.hpp"
#include "InariKonKon/Graphics/Model/Model.hpp"

namespace ikk
{
    class GraphicsPipeline final : public VkType<VkPipeline>
    {
    public:
        template <class VertexType, class IndiciesType>
        GraphicsPipeline(LogicalDevice& logicalDevice, Renderpass& renderpass, const Model<VertexType, IndiciesType>& model) noexcept;

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

        void create(Renderpass& renderpass, const VulkanShader& fragment, const VulkanShader& vertex,
            const VkVertexInputBindingDescription& bindingDescription, const std::vector<VkVertexInputAttributeDescription>& attributeDescriptions) noexcept;

        friend class CommandBuffer;
    };

    template <class VertexType, class IndiciesType>
    GraphicsPipeline::GraphicsPipeline(LogicalDevice& logicalDevice, Renderpass& renderpass, const Model<VertexType, IndiciesType>& model) noexcept
        : m_logicalDevice(&logicalDevice)
    {
        VulkanShader VkFragmentShader { *this->m_logicalDevice, model.getFragmentShader() };
        VulkanShader VkVertexShader { *this->m_logicalDevice, model.getVertexShader() };

        const auto bindingDescription = [](const VertexInfo& info) noexcept
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = info.binding;
            bindingDescription.stride = info.stride;
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            switch (info.inputRate)
            {
            case VertexInfo::InputRate::Per_Vertex:
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                break;
            case VertexInfo::InputRate::Per_Instance:
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
                break;
            }
            return bindingDescription;
        }(model.getVertexInfo());

        const auto attributeDescriptions = [](const VertexInfo& info) noexcept
        {
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions{ info.attributes.size() };

            for (std::size_t i = 0; i < attributeDescriptions.size(); ++i)
            {
                attributeDescriptions[i].binding = info.attributes.at(i).binding;
                attributeDescriptions[i].location = info.attributes.at(i).location;

                switch (info.attributes.at(i).format)
                {
                case VertexAttributes::Format::Float:
                    attributeDescriptions[i].format = VK_FORMAT_R32_SFLOAT;
                    break;
                case VertexAttributes::Format::Vec2:
                    attributeDescriptions[i].format = VK_FORMAT_R32G32_SFLOAT;
                    break;
                case VertexAttributes::Format::Vec3:
                    attributeDescriptions[i].format = VK_FORMAT_R32G32B32_SFLOAT;
                    break;
                case VertexAttributes::Format::Vec4:
                    attributeDescriptions[i].format = VK_FORMAT_R32G32B32A32_SFLOAT;
                    break;
                }
                    
                attributeDescriptions[i].offset = info.attributes.at(i).offset;
            }

            return attributeDescriptions;
        }(model.getVertexInfo());

        this->create(renderpass, VkFragmentShader, VkVertexShader, bindingDescription, attributeDescriptions);
    }
}