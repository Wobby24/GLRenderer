#include "VR_Engine.hpp"
#include "Renderer/OpenGL/GLRenderer.hpp"
#include "Platform/Window/GLFW_Window.hpp"
#include "Platform/Input/GLFW_InputService.hpp"
#include "Scene/VR_Test_Scene.hpp"

int main() {
    VR_Engine::VR_Engine engine;

    auto renderer = std::make_unique<Renderer::OpenGL::GLRenderer>();
    auto window = std::make_unique<Platform::Window::GLFW_Window>();
    auto inputService = std::make_unique<Platform::Input::GLFW_InputService>(
        static_cast<GLFWwindow*>(window->GetNativeHandle())
    );

    
    auto scene = std::make_unique<VR_Engine::Scene::VR_Test_Scene>();

    engine.setRenderer(std::move(renderer));
    engine.setWindow(std::move(window));
    engine.setInputService(std::move(inputService));

    engine.setScene(std::move(scene));

    engine.initialize();
    engine.run();
    engine.shutdown();

    return 0;
}
