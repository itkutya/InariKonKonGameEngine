#include "InariKonKon/InariKonKon.hpp"

class TestApp : public ikk::Application
{
public:
    TestApp() noexcept : ikk::Application(u8"日本", 800, 600)
    {
    }

    void onEvent(const ikk::Event& event) noexcept override
    {
    }

    void onUpdate(const ikk::Time& dt) noexcept override
    {
    }

    void onRender(const ikk::Window& window) noexcept override
    {
        window.draw(ikk::Model<int, int>{fragment, vertex, {}, {} });
    }
private:
    ikk::Shader vertex{ ikk::Filesystem{ "/home/itkutya/Documents/c++/InariKonKonGameEngine/shaders/shader.vert" }, ikk::Shader::VERTEX };
    ikk::Shader fragment { ikk::Filesystem{ "/home/itkutya/Documents/c++/InariKonKonGameEngine/shaders/shader.frag" }, ikk::Shader::FRAGMENT };
};

int main()
{
    TestApp app{};
    app.run();
    return 0;
}