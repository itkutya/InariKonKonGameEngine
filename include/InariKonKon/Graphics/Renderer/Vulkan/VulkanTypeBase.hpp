#ifndef IKK_VULKAN_TYPE_HPP
#define IKK_VULKAN_TYPE_HPP

#include <utility>

#include "vulkan/vulkan.h"

namespace ikk
{
    template<class T>
    class VkType
    {
    public:
        VkType() noexcept = default;
        explicit VkType(T&& type) noexcept;

        virtual ~VkType() noexcept = default;

        VkType(const VkType&) noexcept = default;
        VkType(VkType&&) noexcept = default;

        VkType& operator=(const VkType&) noexcept = default;
        VkType& operator=(VkType&&) noexcept = default;

        template<class Self>
        [[nodiscard]] auto&& getUnderlyingVkType(this Self&& self) noexcept;
    protected:
        T m_type = VK_NULL_HANDLE;
    private:
    };

    template<class T>
    VkType<T>::VkType(T&& type) noexcept : m_type(std::move(type))
    {
    }

    template<class T>
    template<class Self>
    auto&& VkType<T>::getUnderlyingVkType(this Self&& self) noexcept
    {
        return std::forward<decltype(self)>(self).m_type;
    }
}

#endif