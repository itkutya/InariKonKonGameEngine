#include "InariKonKon/Application/Application.hpp"

#include <thread>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "InariKonKon/Utility/Log.hpp"

namespace ikk
{
    Application::Application(std::u8string_view title, const std::uint32_t width, const std::uint32_t height, const Engine engine) noexcept
        : m_window(title, width, height, engine), m_clock({})
    {
        DEBUG_LOG("Application created.", Log::INFO, Log::ALL);
    }

    Application::~Application() noexcept
    {
        glfwTerminate();
        DEBUG_LOG("Application destroyed.", Log::INFO, Log::ALL);
    }

    void Application::run() noexcept
    {
        while (!this->m_window.shouldClose())
        {
            this->handelEvents();
            this->update();
            this->render();
            this->sleep(this->m_fps);
        }
    }

    void Application::handelEvents() noexcept
    {
        this->m_window.pollEvents();

        for (const Event& event : this->m_window.getEventQueue())
            this->onEvent(event);
        
        this->m_window.getEventQueue().clear();
    }

    void Application::update() noexcept
    {
        this->onUpdate(this->m_deltaTime);
    }

    void Application::render() noexcept
    {
        this->m_window.getRenderer()->beginRender();
        this->onRender(this->m_window);
        this->m_window.getRenderer()->endRender();
    }

    void Application::sleep(const std::uint32_t fps_limit) noexcept
    {
        if (fps_limit > 0)
        {
            const Time targetFPS = microseconds(static_cast<std::uint64_t>(1000000U / fps_limit));
            //TODO:
            //Make sure it does not block other threads...
            std::this_thread::sleep_for(targetFPS.toDuration());
        }
        this->m_deltaTime = this->m_clock.restart();
    }
}