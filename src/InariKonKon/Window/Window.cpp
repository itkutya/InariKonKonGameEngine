#include "InariKonKon/Window/Window.hpp"

#include <exception>
#include <print>

#define GLFW_INCLUDE_VULKAN
#include "glfw/glfw3.h"

#include "InariKonKon/Renderer/Vulkan.hpp"

namespace ikk
{
    Window::Window(std::u8string_view title, const std::uint32_t width, const std::uint32_t height)
    try : m_window(createWindow(title, width, height)), m_renderer(std::make_shared<Vulkan>(title, m_window, width, height))
    {
        glfwSetWindowUserPointer(this->m_window, this);
        this->setupWindowCallbacks();
    }
    catch(const std::exception& e)
    {
        std::print("{}\n", e.what());
    }

    Window::~Window() noexcept
    {
        glfwDestroyWindow(this->m_window);
    }

    void Window::pollEvents() noexcept
    {
        glfwPollEvents();
    }

    const bool Window::shouldClose() const noexcept
    {
        return static_cast<bool>(glfwWindowShouldClose(this->m_window));
    }

    void Window::draw() const noexcept
    {
        //TODO:
        //add to drawQueue...
    }

    GLFWwindow *Window::createWindow(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height)
    {
        if (!glfwInit())
        {
            std::print("Cannot initialize GLFW.\nExiting ...\n");
            throw std::runtime_error("Cannot initialize GLFW.");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        return glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), reinterpret_cast<const char*>(title.data()), NULL, NULL);
    }

    void Window::setupWindowCallbacks() noexcept
    {
        //TODO:
        //Impl. the rest...
        static auto windowResizeCallback = [](GLFWwindow* window, int width, int height) noexcept
        {
            Window* app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            app->getEventQueue().emplace_back(Event::WindowResized{ .width = static_cast<std::uint32_t>(width), .height = static_cast<std::uint32_t>(height) });
        };
        glfwSetWindowSizeCallback(this->m_window, windowResizeCallback);

        static auto framebufferResizeCallback = [](GLFWwindow* window, int width, int height) noexcept
        {
            Window* app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            if (app != nullptr)
                app->getRenderer()->onResize(static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height));
            
            app->getEventQueue().emplace_back(Event::WindowFramebufferResized{ .width = static_cast<std::uint32_t>(width), .height = static_cast<std::uint32_t>(height) });
        };
        glfwSetFramebufferSizeCallback(this->m_window, framebufferResizeCallback);

        static auto windowClosedCallback = [](GLFWwindow* window) noexcept
        {
            Window* app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            app->getEventQueue().emplace_back(Event::WindowClosed{ });
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