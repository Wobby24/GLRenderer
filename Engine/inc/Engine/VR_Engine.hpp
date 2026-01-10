#include <memory>
#include <Renderer/Interface/IRenderer.hpp>
#include <Platform/Window/Interface/IWindow.hpp>
#include <Platform/Input/Interface/IInputService.hpp>
#include <Scene/Interface/IScene.hpp>

namespace VR_Engine {

    struct EngineSystemScenes {
        std::unique_ptr<Renderer::IRenderScene> renderScene_;
    };

    class VR_Engine {
    public:
        VR_Engine() = default;
        ~VR_Engine() = default;

        void setScene(std::unique_ptr<Scene::IScene> sc) { scene_ = std::move(sc); }

        // Setters accept unique_ptr for ownership transfer
        void setRenderer(std::unique_ptr<Renderer::IRenderer> r) { renderer_ = std::move(r); }
        void setWindow(std::unique_ptr<Platform::Window::IWindow> w) { window_ = std::move(w); }
        void setInputService(std::unique_ptr<Platform::Input::IInputService> i) { inputService_ = std::move(i); }
       

        void initialize();
        void run();
        void update(float deltaTime);
        void shutdown();

    private:
        std::unique_ptr<Scene::IScene> scene_;
        std::unique_ptr<Renderer::IRenderer> renderer_;
        std::unique_ptr<Platform::Window::IWindow> window_;
        std::unique_ptr<Platform::Input::IInputService> inputService_;

        EngineSystemScenes systemScenes;
    };

} // namespace VR_Engine
