#include "InariKonKon/Application/Window/Window.hpp"

#include <exception>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "InariKonKon/Utility/Log.hpp"

namespace ikk
{
    Window::Window(std::u8string_view title, const std::uint32_t width, const std::uint32_t height)
    try : m_window(createWindow(title, width, height)), m_renderer(std::make_shared<Vulkan>(title, m_window, width, height)), m_title(title)
    {
        glfwSetWindowUserPointer(this->m_window, this);
        this->setupWindowCallbacks();

        DEBUG_LOG(fmt::format("{} (window) created.", TO_ANSI(title.data())), Log::INFO, Log::ALL);
    }
    catch(const std::exception& e)
    {
        Log(e.what(), Log::FATAL, Log::ALL);
    }

    Window::~Window() noexcept
    {
        glfwDestroyWindow(this->m_window);

        DEBUG_LOG(fmt::format("{} (window) destroyed.", TO_ANSI(this->m_title.data())), Log::INFO, Log::ALL);
    }

    void Window::pollEvents() noexcept
    {
        glfwPollEvents();
    }

    const bool Window::shouldClose() const noexcept
    {
        return BOOL(glfwWindowShouldClose(this->m_window));
    }

    GLFWwindow *Window::createWindow(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height)
    {
        if (!glfwInit())
        {
            Log("Cannot initialize GLFW.\nExiting ...", Log::FATAL, Log::ALL);
            throw std::runtime_error("Cannot initialize GLFW.");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        return glfwCreateWindow(INT(width), INT(height), TO_ANSI(title.data()), NULL, NULL);
    }

    void Window::setupWindowCallbacks() noexcept
    {
        //TODO:
        //Impl. the rest...

        //Window resize event
        static auto windowResizeCallback = [](GLFWwindow* window, int width, int height) noexcept
        {
            Window* app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            if (app == nullptr)
                return;
            
            app->getEventQueue().emplace_back(Event::WindowResized{ .width = U32(width), .height = U32(height) });
        };
        glfwSetWindowSizeCallback(this->m_window, windowResizeCallback);

        //Framebuffer resize event
        static auto framebufferResizeCallback = [](GLFWwindow* window, int width, int height) noexcept
        {
            Window* app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            if (app == nullptr)
                return;
            
            app->getEventQueue().emplace_back(Event::WindowFramebufferResized{ .width = U32(width), .height = U32(height) });
            app->getRenderer()->onResize(U32(width), U32(height));
        };
        glfwSetFramebufferSizeCallback(this->m_window, framebufferResizeCallback);

        //Window closed event
        static auto windowClosedCallback = [](GLFWwindow* window) noexcept
        {
            Window* app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            if (app == nullptr)
                return;
            
            app->getEventQueue().emplace_back(Event::WindowClosed{});
        };
        glfwSetWindowCloseCallback(this->m_window, windowClosedCallback);
    }

    const std::deque<Event> &Window::getEventQueue() const noexcept
    {
        return this->m_eventQueue;
    }

    std::deque<Event> &Window::getEventQueue() noexcept
    {
        return this->m_eventQueue;
    }

    const std::shared_ptr<RendererBase> &Window::getRenderer() const noexcept
    {
        return this->m_renderer;
    }

    std::shared_ptr<RendererBase>& Window::getRenderer() noexcept
    {
        return this->m_renderer;
    }
}