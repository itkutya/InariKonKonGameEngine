#include "InariKonKon/Graphics/Renderer/Vulkan/GraphicsPipeline.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan/Shader/VkShader.hpp"

namespace ikk
{
    GraphicsPipeline::GraphicsPipeline(LogicalDevice& logicalDevice, Renderpass& renderpass, const Shader& fragment, const Shader& vertex) noexcept
        : m_logicalDevice(&logicalDevice)
    {
        VkShader VkFragmentShader { *this->m_logicalDevice, fragment };
        VkShader VkVertexShader { *this->m_logicalDevice, vertex };

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = VkVertexShader.getUnderlyingVkType();
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = VkFragmentShader.getUnderlyingVkType();
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        const Shader::VertexInfo info = vertex.createVertexInfo();
        //TODO:
        //Own func (?)
        const auto bindingDescription = [](const Shader::VertexInfo& info) noexcept
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = info.binding;
            bindingDescription.stride = info.stride;
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            switch (info.inputRate)
            {
            case Shader::VertexInfo::InputRate::Per_Vertex:
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                break;
            case Shader::VertexInfo::InputRate::Per_Instance:
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
                break;
            }
            return bindingDescription;
        }(info);

        const auto attributeDescriptions = [](const Shader::VertexInfo& info) noexcept
        {
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions{ info.attributes.size() };

            for (std::size_t i = 0; i < attributeDescriptions.size(); ++i)
            {
                attributeDescriptions[i].binding = info.attributes.at(i).binding;
                attributeDescriptions[i].location = info.attributes.at(i).location;

                switch (info.attributes.at(i).format)
                {
                case Shader::VertexAttributes::Format::Float:
                    attributeDescriptions[i].format = VK_FORMAT_R32_SFLOAT;
                    break;
                case Shader::VertexAttributes::Format::Vec2:
                    attributeDescriptions[i].format = VK_FORMAT_R32G32_SFLOAT;
                    break;
                case Shader::VertexAttributes::Format::Vec3:
                    attributeDescriptions[i].format = VK_FORMAT_R32G32B32_SFLOAT;
                    break;
                case Shader::VertexAttributes::Format::Vec4:
                    attributeDescriptions[i].format = VK_FORMAT_R32G32B32A32_SFLOAT;
                    break;
                }
                    
                attributeDescriptions[i].offset = info.attributes.at(i).offset;
            }

            return attributeDescriptions;
        }(info);

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        std::vector<VkDynamicState> dynamicStates =
        {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<std::uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        VK_CHECK(vkCreatePipelineLayout(this->m_logicalDevice->getUnderlyingVkType(), &pipelineLayoutInfo, nullptr, &this->m_pipelineLayout));

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = this->m_pipelineLayout;
        pipelineInfo.renderPass = renderpass.getUnderlyingVkType();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateGraphicsPipelines(this->m_logicalDevice->getUnderlyingVkType(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->m_type));
        DEBUG_LOG("Vulkan graphics pipeline created.", Log::INFO, Log::ALL);
    }
    
    GraphicsPipeline::~GraphicsPipeline() noexcept
    {
        vkDestroyPipeline(this->m_logicalDevice->getUnderlyingVkType(), this->m_type, nullptr);
        vkDestroyPipelineLayout(this->m_logicalDevice->getUnderlyingVkType(), this->m_pipelineLayout, nullptr);
        DEBUG_LOG("Vulkan graphics pipeline destroyed.", Log::INFO, Log::ALL);
    }
    
    void GraphicsPipeline::bind(VkCommandBuffer &commandBuffer, const VkPipelineBindPoint pipelineBindPoint) noexcept
    {
        vkCmdBindPipeline(commandBuffer, pipelineBindPoint, this->m_type);
    }
}