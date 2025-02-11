#include "InariKonKon/Application/Application.hpp"

#define GLFW_INCLUDE_VULKAN
#include "glfw/glfw3.h"

#include "InariKonKon/Utility/Utility.hpp"

namespace ikk
{
    Application::Application(std::u8string_view title, const std::uint32_t width, const std::uint32_t height) noexcept
        : m_window(title, width, height), m_clock({})
    {
    }

    Application::~Application() noexcept
    {
        glfwTerminate();
    }

    void Application::run() noexcept
    {
        while (!this->m_window.shouldClose())
        {
            BENCHMARK_BLOCK("MAIN LOOP");
            
            BENCHMARK_BEGIN("EVENT HANDLE");
            this->handelEvents();
            BENCHMARK_END();

            BENCHMARK_BEGIN("UPDATE");  
            this->onUpdate(this->m_clock.getElapsedTime());
            BENCHMARK_END();
            
            BENCHMARK_BEGIN("RENDER");
            this->render();
            BENCHMARK_END();
            
            BENCHMARK_BEGIN("SLEEP");
            //TODO:
            //Make it variable...
            this->sleep(60);
            BENCHMARK_END();
        }
    }

    void Application::handelEvents() noexcept
    {
        this->m_window.pollEvents();
        std::deque<Event>& eventQueue = this->m_window.getEventQueue();
        for (const Event& event : eventQueue)
            this->onEvent(event);
        eventQueue.clear();
    }

    void Application::render() noexcept
    {
        this->onRender(this->m_window);
        //TODO:
        //Fix this shit...
        this->m_window.getRenderer()->beginRender(Viewport{ this->m_window.getRenderer()->getSurface(), 0.f, 1.f },
            { 0, 0, (std::uint32_t)this->m_window.getRenderer()->getSurface().width, (std::uint32_t)this->m_window.getRenderer()->getSurface().height });
        this->m_window.getRenderer()->endRender();
    }

    void Application::sleep(const std::uint32_t fps_limit) noexcept
    {
        if (fps_limit > 0)
        {
            const Time targetFPS = microseconds(static_cast<std::uint64_t>(1000000U / fps_limit));
            while (targetFPS > this->m_clock.getElapsedTime());
            this->m_clock.restart();
        }
    }
}