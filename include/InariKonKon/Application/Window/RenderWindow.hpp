#pragma once

#include "InariKonKon/Application/Window/Window.hpp"

//#include "InariKonKon/Graphics/Renderer/RendererBase.hpp"
//TODO:
//Remove somehow...
#include "InariKonKon/Graphics/Renderer/Vulkan.hpp"

namespace ikk
{
    class RenderWindow : public Window
    {
    public:
        RenderWindow(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height, const Engine engine) noexcept;

        ~RenderWindow() noexcept = default;

        RenderWindow(const RenderWindow&) noexcept = default;
        RenderWindow(RenderWindow&&) noexcept = default;

        RenderWindow& operator=(const RenderWindow&) noexcept = default;
        RenderWindow& operator=(RenderWindow&&) noexcept = default;

        template<class VerteciesType, class IndiciesType = void*>
        void draw(const Model<VerteciesType, IndiciesType>* model) const noexcept;
    protected:
        void onResize([[maybe_unused]] const std::uint32_t width, [[maybe_unused]] const std::uint32_t height) noexcept override;
    private:
        std::shared_ptr<RendererBase> m_renderer;
        Engine m_engine;

        const std::shared_ptr<RendererBase>& getRenderer() const noexcept;
        std::shared_ptr<RendererBase>& getRenderer() noexcept;

        std::shared_ptr<RendererBase> createRenderer(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height, const Engine engine) const noexcept;

        friend class Application;
    };

    template <class VerteciesType, class IndiciesType>
    void RenderWindow::draw(const Model<VerteciesType, IndiciesType>* model) const noexcept
    {
        //TODO:
        //Make OpenGL renderer as well...
        switch (this->m_engine)
        {
        case Engine::None:
            break;
        case Engine::Vulkan:
            static_cast<Vulkan*>(this->m_renderer.get())->draw(model);
            break;
        }
    }
}