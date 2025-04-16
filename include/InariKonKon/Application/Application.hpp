#ifndef IKK_APPLICATION_HPP
#define IKK_APPLICATION_HPP

#include "InariKonKon/Application/Window/RenderWindow.hpp"
#include "InariKonKon/Utility/Clock.hpp"

namespace ikk
{
    class Application
    {
    public:
        //TODO:
        //Save application size & info
        //Application(std::u8string_view title) noexcept;
        Application(std::u8string_view title, const std::uint32_t width, const std::uint32_t height, const Engine engine) noexcept;

        virtual ~Application() noexcept = default;

        Application(const Application&) noexcept = default;
        Application(Application&&) noexcept = default;

        Application& operator=(const Application&) noexcept = default;
        Application& operator=(Application&&) noexcept = default;

        virtual void onEvent(const Event& event) noexcept = 0;
        virtual void onUpdate(const Time& dt) noexcept = 0;
        virtual void onRender(const RenderWindow& window) noexcept = 0;

        virtual void run() noexcept final;
    private:
        RenderWindow m_window;
        Clock m_clock;
        Time m_deltaTime;

        std::uint32_t m_fps = 60;

        void handelEvents() noexcept;
        void update() noexcept;
        void render() noexcept;
        void sleep(const std::uint32_t fps_limit) noexcept;
    };
}

#endif