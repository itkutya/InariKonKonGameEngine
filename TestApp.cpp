#include "InariKonKon/InariKonKon.hpp"

class TestApp : public ikk::Application
{
public:
    TestApp() noexcept : ikk::Application(u8"日本", 800, 600, ikk::Engine::Vulkan)
    {
    }

    void onEvent(const ikk::Event& event) noexcept override
    {
    }

    void onUpdate(const ikk::Time& dt) noexcept override
    {
    }

    void onRender(const ikk::RenderWindow& window) noexcept override
    {
        window.draw(&model1);
        //window.draw(model2);
    }
private:
    inline static ikk::Shader vertex{ ikk::Filesystem{ "/home/itkutya/Documents/c++/InariKonKonGameEngine/shaders/shader.vert" }, ikk::Shader::VERTEX };
    inline static ikk::Shader fragment { ikk::Filesystem{ "/home/itkutya/Documents/c++/InariKonKonGameEngine/shaders/shader.frag" }, ikk::Shader::FRAGMENT };
    inline static ikk::Model<ikk::UIVertex> model1 { fragment, vertex, {   
                                                                        { { -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }},
                                                                        { { -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }},
                                                                        { {  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }},
                                                                        { {  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }}
                                                                       }
                                                   };
    //inline static ikk::Model<TestVertex, std::uint32_t> model2{fragment, vertex, {}, {} };
};

int main()
{
    TestApp app{};
    app.run();
    return 0;
}