#pragma once

#include <utility>

#include "vulkan/vulkan.h"

namespace ikk
{
    template<class T>
    class VkType
    {
    public:
        VkType() noexcept = default;

        virtual ~VkType() noexcept = default;

        VkType(const VkType&) noexcept = default;
        VkType(VkType&&) noexcept = default;

        VkType& operator=(const VkType&) noexcept = default;
        VkType& operator=(VkType&&) noexcept = default;

        template<class Self>
        [[nodiscard]] inline auto&& getUnderlyingVkType(this Self&& self) noexcept
        {
            return std::forward<decltype(self)>(self).m_type;
        }
    protected:
        T m_type = VK_NULL_HANDLE;
    private:
    };
}