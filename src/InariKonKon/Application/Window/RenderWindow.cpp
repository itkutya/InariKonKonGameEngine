#include "InariKonKon/Application/Window/RenderWindow.hpp"

#include "InariKonKon/Graphics/Renderer/Vulkan.hpp"

namespace ikk
{
    [[nodiscard]] std::shared_ptr<RendererBase> createRenderer(const std::u8string_view title, GLFWwindow* window,
        const std::uint32_t width, const std::uint32_t height, const Engine engine) noexcept
    {
        switch (engine)
        {
        case Engine::None:
            return nullptr;
        case Engine::Vulkan:
            return std::make_shared<Vulkan>(title, window, width, height);
        }
        return nullptr;
    }

    RenderWindow::RenderWindow(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height, const Engine engine) noexcept
        : Window(title, width, height), m_renderer(createRenderer(title, m_window, width, height, engine)), m_engine(engine)
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
}