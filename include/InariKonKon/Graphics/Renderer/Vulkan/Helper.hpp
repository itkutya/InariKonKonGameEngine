#ifndef IKK_VULKAN_HELPER_HPP
#define IKK_VULKAN_HELPER_HPP

#include <exception>

#include "InariKonKon/Utility/Utility.hpp"
#include "InariKonKon/Utility/Log.hpp"

#ifdef IKK_DEBUG
    #include "vulkan/vk_enum_string_helper.h"
    
    #define VK_CHECK(x)                                         \
        try                                                     \
        {                                                       \
            const VkResult err = x;                             \
            if (err != VK_SUCCESS)                              \
                throw std::runtime_error(string_VkResult(err)); \
        }                                                       \
        catch (const std::exception& e)                         \
        {                                                       \
            ikk::Log(e.what(), Log::FATAL, Log::ALL);           \
        } ((void)0)
#else
    #define VK_CHECK(x) x
#endif

#endif