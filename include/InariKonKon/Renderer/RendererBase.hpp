#pragma once

#include <cstdint>

#include "InariKonKon/Window/Viewport.hpp"

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
        //TODO:
        //Make a scissor type...
        virtual void beginRender(const Viewport& viewport, const u32Rect& scissor) = 0;
        virtual void endRender() = 0;

        virtual const fRect getSurface() const noexcept = 0;
    private:
        friend class Window;
        friend class Application;
    };
}