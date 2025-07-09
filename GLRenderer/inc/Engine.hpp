#include <memory>
#include <GLRenderer/OpenGL/GLRenderer.hpp>
#include <GLRenderer/Window/Window.hpp>

class Engine {
public:
    Engine() = default;
    ~Engine() = default;

    void initialize();
    void run();
    void update(float deltaTime);
    void shutdown();

private:
    std::unique_ptr<GLRenderer::GLRenderer> renderer_;
    std::unique_ptr<GLRenderer::Window::Window> window_;
};
