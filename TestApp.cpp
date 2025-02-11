#include <print>

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
        //std::print("Hello from TestApp update\n");
    }

    void onRender(const ikk::Window& window) noexcept override
    {
        window.draw(/*object*/);
    }
private:
};


int main()
{
    TestApp app{};
    app.run();
    return 0;
}