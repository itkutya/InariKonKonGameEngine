#include "InariKonKon/Graphics/Renderer/Vulkan/Instance.hpp"

#include <string>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "InariKonKon/Graphics/Renderer/Vulkan/Helper.hpp"

namespace ikk
{
#ifdef IKK_DEBUG
    //TODO:
    //Move it somewhere else...
    class VulkanValidationLayer final : public ikk::VkType<VkDebugUtilsMessengerEXT>
    {
    public:
        VulkanValidationLayer() noexcept = default;

        ~VulkanValidationLayer() noexcept = default;

        VulkanValidationLayer(const VulkanValidationLayer&) noexcept = default;
        VulkanValidationLayer(VulkanValidationLayer&&) noexcept = default;

        VulkanValidationLayer& operator=(const VulkanValidationLayer&) noexcept = default;
        VulkanValidationLayer& operator=(VulkanValidationLayer&&) noexcept = default;

        [[nodiscard]] const VkResult create(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo) noexcept;
        void destroy(VkInstance instance) noexcept;

        [[nodiscard]] const bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers) const noexcept;
    private:
    };

    const VkResult VulkanValidationLayer::create(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo) noexcept
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
            return func(instance, pCreateInfo, nullptr, &this->m_type);
        else
            return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void VulkanValidationLayer::destroy(VkInstance instance) noexcept
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
            func(instance, this->m_type, nullptr);
    }

    const bool VulkanValidationLayer::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) const noexcept
    {
        std::uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers{ static_cast<std::size_t>(layerCount) };
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        std::set<std::string> requiredValidationLayers{ validationLayers.begin(), validationLayers.end() };
        for (const char* layerName : validationLayers)
            requiredValidationLayers.erase(layerName);
        
        return requiredValidationLayers.empty();
    }

    inline static VulkanValidationLayer s_validationLayer{};
#endif

    Instance::Instance(const std::u8string_view title)
    try
    {
        //TODO:
        //- Add version control to the user side
        //- Add version control to the engine side (aka CMake version control)
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = reinterpret_cast<const char*>(title.data());
        appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        appInfo.pEngineName = "InariKonKon Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        const std::vector<const char*>& extensions = this->getRequiredExtensions();

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = U32(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        #ifdef IKK_DEBUG
            if (!s_validationLayer.checkValidationLayerSupport(this->getValidationLayers()))
                throw std::runtime_error("Validation layers requested, but not available!");
            
            const std::vector<const char*>& validationLayers = this->getValidationLayers();

            createInfo.enabledLayerCount = U32(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugCreateInfo.messageSeverity = 
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT   |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
            debugCreateInfo.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

            static auto debugCallback = [](
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) noexcept
            {
                if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
                    Log(fmt::format("(Validation Layer): {}", pCallbackData->pMessage), Log::ERROR);
                else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
                    Log(fmt::format("(Validation Layer): {}", pCallbackData->pMessage), Log::WARNING);
                else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
                    Log(fmt::format("(Validation Layer): {}", pCallbackData->pMessage), Log::INFO);
                else
                    Log(fmt::format("(Validation Layer): {}", pCallbackData->pMessage));
                return VK_FALSE;
            };
            
            debugCreateInfo.pfnUserCallback = debugCallback;
            createInfo.pNext = (void*)&debugCreateInfo;

            VK_CHECK(vkCreateInstance(&createInfo, nullptr, &this->m_type));
            VK_CHECK(s_validationLayer.create(this->m_type, &debugCreateInfo));
        #else
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
                
            VK_CHECK(vkCreateInstance(&createInfo, nullptr, &this->m_type));
        #endif

        DEBUG_LOG("Vulkan instance created.", Log::INFO, Log::ALL);
    }
    catch(const std::exception& e)
    {
        Log(e.what(), Log::FATAL, Log::ALL);
    }

    Instance::~Instance() noexcept
    {
        #ifdef IKK_DEBUG
            s_validationLayer.destroy(this->m_type);
        #endif
        vkDestroyInstance(this->m_type, nullptr);

        DEBUG_LOG("Vulkan instance Destroyed.", Log::INFO, Log::ALL);
    }

    const std::vector<const char*> Instance::getValidationLayers() noexcept
    {
        #ifdef IKK_DEBUG
            return std::vector<const char*>{ "VK_LAYER_KHRONOS_validation" };
        #else
            return std::vector<const char*>{};
        #endif
    }

    const std::vector<const char*> Instance::getRequiredExtensions() noexcept
    {
        std::uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions { glfwExtensions, glfwExtensions + glfwExtensionCount };

        #ifdef IKK_DEBUG
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        #endif

        return extensions;
    }
}