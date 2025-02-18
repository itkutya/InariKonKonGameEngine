#pragma once

#include "InariKonKon/Window/Window.hpp"
#include "InariKonKon/Utility/Clock.hpp"

#include "InariKonKon/Entitiy/Entity.hpp"

namespace ikk
{
    class Application
    {
    public:
        Application(std::u8string_view title, const std::uint32_t width, const std::uint32_t height) noexcept;

        virtual ~Application() noexcept;

        Application(const Application&) noexcept = default;
        Application(Application&&) noexcept = default;

        Application& operator=(const Application&) noexcept = default;
        Application& operator=(Application&&) noexcept = default;

        virtual void onEvent(const Event& event) noexcept = 0;
        virtual void onUpdate(const Time& dt) noexcept = 0;
        virtual void onRender(const Window& window) noexcept = 0;

        virtual void run() noexcept final;
    private:
        Window m_window;
        Clock m_clock;

        std::vector<Entity> m_entities;

        void handelEvents() noexcept;
        void render() noexcept;
        void sleep(const std::uint32_t fps_limit) noexcept;
    };
}