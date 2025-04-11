#ifndef IKK_WINDOW_HPP
#define IKK_WINDOW_HPP

#include <string_view>
#include <memory>
#include <queue>

#include "InariKonKon/Application/Window/Event/Event.hpp"

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
    protected:
        virtual void onResize([[maybe_unused]] const std::uint32_t width, [[maybe_unused]] const std::uint32_t height) noexcept;
        
        GLFWwindow* m_window;
    private:
        std::u8string_view m_title;
        std::deque<Event> m_eventQueue;

        GLFWwindow* createWindow(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height);
        void setupWindowCallbacks() noexcept;

        const std::deque<Event>& getEventQueue() const noexcept;
        std::deque<Event>& getEventQueue() noexcept;

        friend class Application;
    };
}

#endif