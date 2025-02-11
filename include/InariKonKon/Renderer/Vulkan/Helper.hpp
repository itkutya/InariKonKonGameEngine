#pragma once

#include <exception>

#include "vulkan/vk_enum_string_helper.h"

#include "InariKonKon/Utility/Utility.hpp"

#ifdef IKK_DEBUG
    #define VK_CHECK(x)                                                                                 \
        {                                                                                               \
            const VkResult err = x;                                                                     \
            if (err != VK_SUCCESS)                                                                      \
            {                                                                                           \
                ikk::log(std::format("Vulkan ERROR: {}", string_VkResult(err)), ikk::FATAL); \
                throw std::runtime_error(string_VkResult(err));                                         \
            }                                                                                           \
        } ((void)0)
#else
    #define VK_CHECK(x) x
#endif