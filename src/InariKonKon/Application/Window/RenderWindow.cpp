#include "InariKonKon/Application/Window/RenderWindow.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan.hpp"

namespace ikk
{
    RenderWindow::RenderWindow(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height, const Engine engine) noexcept
        : Window(title, width, height), m_renderer(createRenderer(title, width, height, engine)), m_engine(engine)
    {
    }

    void RenderWindow::onResize(const std::uint32_t width, const std::uint32_t height) noexcept
    {
        this->m_renderer->onResize(width, height);
    }

    const std::shared_ptr<RendererBase> &RenderWindow::getRenderer() const noexcept
    {
        return this->m_renderer;
    }

    std::shared_ptr<RendererBase>& RenderWindow::getRenderer() noexcept
    {
        return this->m_renderer;
    }

    std::shared_ptr<RendererBase> RenderWindow::createRenderer(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height, const Engine engine) const noexcept
    {
        switch (engine)
        {
        case Engine::None:
            return nullptr;
        case Engine::Vulkan:
            return std::make_shared<Vulkan>(title, m_window, width, height);
        }
        return nullptr;
    }
}