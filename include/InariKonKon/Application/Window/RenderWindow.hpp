#ifndef IKK_RENDER_WINDOW_HPP
#define IKK_RENDER_WINDOW_HPP

#include "InariKonKon/Application/Window/Window.hpp"

#include "InariKonKon/Graphics/Renderer/RendererBase.hpp"
#include "InariKonKon/Graphics/Model/Model.hpp"

namespace ikk
{
    struct RenderObject
    {
        const void* data;
        std::size_t size;
        Shader vertexShader;
        Shader fragmentShader;
        VertexInfo vertexInfo;
    };

    class RenderWindow : public Window
    {
    public:
        RenderWindow(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height, const Engine engine) noexcept;

        ~RenderWindow() noexcept = default;

        RenderWindow(const RenderWindow&) noexcept = default;
        RenderWindow(RenderWindow&&) noexcept = default;

        RenderWindow& operator=(const RenderWindow&) noexcept = default;
        RenderWindow& operator=(RenderWindow&&) noexcept = default;

        template<class VerteciesType, class IndiciesType = void*>
        void draw(const Model<VerteciesType, IndiciesType>* model) const noexcept;
    protected:
        void onResize([[maybe_unused]] const std::uint32_t width, [[maybe_unused]] const std::uint32_t height) noexcept override;
    private:
        std::shared_ptr<RendererBase> m_renderer;
        Engine m_engine;

        const std::shared_ptr<RendererBase>& getRenderer() const noexcept;
        std::shared_ptr<RendererBase>& getRenderer() noexcept;

        std::shared_ptr<RendererBase> createRenderer(const std::u8string_view title, const std::uint32_t width, const std::uint32_t height, const Engine engine) const noexcept;

        friend class Application;
    };

    template <class VerteciesType, class IndiciesType>
    void RenderWindow::draw(const Model<VerteciesType, IndiciesType>* model) const noexcept
    {
        if (model == nullptr)
            return;

        switch (this->m_engine)
        {
        case Engine::None:
            break;
        case Engine::Vulkan:
            RenderObject renderObj =
            {
                .data = static_cast<const void*>(&(*model->getVertecies().begin())),
                .size = model->getVertecies().size() * sizeof(VerteciesType),
                .vertexShader = model->getVertexShader(),
                .fragmentShader = model->getFragmentShader(),
                .vertexInfo = model->getVertexInfo()
            };
            this->m_renderer->draw(renderObj);
            break;
        }
    }
}

#endif