#pragma once

#include "InariKonKon/Model/Rect.hpp"

namespace ikk
{
    class Viewport final
    {
    public:
        Viewport(const fRect rect, const float minDepth, const float maxDepth) noexcept;

        ~Viewport() noexcept = default;

        Viewport(const Viewport&) noexcept = default;
        Viewport(Viewport&&) noexcept = default;

        Viewport& operator=(const Viewport&) noexcept = default;
        Viewport& operator=(Viewport&&) noexcept = default;

        const fRect& getRect() const noexcept;
        const float& getMinDepth() const noexcept;
        const float& getMaxDepth() const noexcept;
    private:
        fRect m_rect;
        float m_minDepth;
        float m_maxDepth;
    };
}