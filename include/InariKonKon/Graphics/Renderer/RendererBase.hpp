#pragma once

#include <cstdint>

#include "InariKonKon/Graphics/Model/Model.hpp"

namespace ikk
{
    class RendererBase
    {
    public:
        RendererBase() noexcept = default;

        virtual ~RendererBase() noexcept = default;

        RendererBase(const RendererBase&) noexcept = default;
        RendererBase(RendererBase&&) noexcept = default;

        RendererBase& operator=(const RendererBase&) noexcept = default;
        RendererBase& operator=(RendererBase&&) noexcept = default;
    protected:
        virtual void onResize(const std::uint32_t width, const std::uint32_t height) noexcept = 0;

        virtual void beginRender() = 0;
        virtual void endRender() = 0;
    private:
        friend class Window;
        friend class Application;
    };
}