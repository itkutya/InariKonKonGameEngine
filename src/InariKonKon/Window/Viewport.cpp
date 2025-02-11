#include "InariKonKon/Window/Viewport.hpp"

namespace ikk
{
    Viewport::Viewport(const fRect rect, const float minDepth, const float maxDepth) noexcept
        : m_rect(rect), m_minDepth(minDepth), m_maxDepth(maxDepth)
    {
    }

    const fRect& Viewport::getRect() const noexcept
    {
        return this->m_rect;
    }

    const float& Viewport::getMinDepth() const noexcept
    {
        return this->m_minDepth;
    }

    const float& Viewport::getMaxDepth() const noexcept
    {
        return this->m_maxDepth;
    }
}