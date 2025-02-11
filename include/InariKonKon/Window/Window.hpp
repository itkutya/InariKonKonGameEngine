#pragma once

#include <string_view>
#include <memory>
#include <queue>

#include "InariKonKon/Event/Event.hpp"
#include "InariKonKon/Renderer/RendererBase.hpp"

struct GLFWwindow;

namespace ikk
{
    class Window
    {
    public:
        Window(std::u8string_view title, const std::uint32_t width, const std::uint32_t height);

        ~Window() noexcept;

        Window(const Window&) noexcept = default;
        Window(Window&&) noexcept = default;

        Window& operator=(const Window&) noexcept = default;
        Window& operator=(Window&&) noexcept = default;

        void pollEvents() noexcept;

        const bool shouldClose() const noexcept;

        void draw() const noexcept;
    private:
        std::u8string_view m_title;
        GLFWwindow* m_window;
        std::deque<Event> m_eventQueue;
        std::shared_ptr<RendererBase> m_renderer;

        GLFWwindow* createWindow(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height);
        void setupWindowCallbacks() noexcept;

        const std::deque<Event>& getEventQueue() const noexcept;
        std::deque<Event>& getEventQueue() noexcept;

        const std::shared_ptr<RendererBase>& getRenderer() const noexcept;
        std::shared_ptr<RendererBase>& getRenderer() noexcept;

        friend class Application;
    };
}