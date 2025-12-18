#include <GLRenderer/OpenGL/GLRenderer.hpp>
#include <GLRenderer/OpenGL/OpenGLInitializer.hpp>
#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRenderState.hpp>
#include <GLRenderer/Window/GLFWRenderSurface.hpp>
#include <GLRenderer/Window/Interface/IRenderSurface.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>  // for glfwGetTime()
#include <stdexcept>

namespace GLRenderer
{

   // GLRenderer::GLRenderer() : isInitialized_(false), isCleanedUp_(false) {}
    
    GLRenderer::~GLRenderer() {
        if (isCleanedUp_) return;
        Cleanup();
    }

    void GLRenderer::Initialize(IRendererContextDesc& contextDesc, IRenderSurface& surface)
    {
        surface_ = &surface; // store pointer

        InitializeDefaults();
        InitializeOpenGL(contextDesc);
        isInitialized_ = true;
        state_.ApplyState();

       // LoadInitialScene(); // <-- put this here instead
        lastTime_ = glfwGetTime();
    }

    void GLRenderer::SetRenderSurface(IRenderSurface* surface) {
        if (!surface) throw std::runtime_error("Surface cannot be null");
        surface_ = surface;
    }

    void GLRenderer::SetWindow(Window::IWindow& window) {
        window_ = &window;
        sceneManager_.setWindow(window);
    }

    void GLRenderer::InitializeDefaults()
    {
        // Set default render state
        state_.clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f); // Default clear color
        state_.depthTestEnabled = true; //most if not all scenes will need this; it can be passed to a scene if it needs changing, otherwise its on.
        // Additional defaults can be set here as needed
    }

    void GLRenderer::InitializeOpenGL(IRendererContextDesc& contextDesc) {
        // Safe runtime type check
        auto* glContextDesc = dynamic_cast<GLRendererContextDesc*>(&contextDesc);
        if (!glContextDesc)
        {
            throw std::runtime_error("GLRenderer: Invalid context descriptor type.");
        }

        contextDesc_ = *glContextDesc;

        if (!OpenGLInitializer::Initialize(*glContextDesc)) {
            throw std::runtime_error("OpenGL initialization failed.");
        }
    }

    void GLRenderer::RenderFrame()
    {
        if (!isInitialized_) {
            throw std::runtime_error("RenderFrame called before GLRenderer was initialized.");
        }

        if (isCleanedUp_) {
            throw std::runtime_error("RenderFrame called after clean-up!");
        }

        if (!surface_) return;

        surface_->Bind();

        double currentTime = glfwGetTime();
        deltaTime = static_cast<float>(currentTime - lastTime_);
        lastTime_ = currentTime;

        sceneManager_.resizeCurrent(surface_->GetSize().x, surface_->GetSize().y);
        sceneManager_.renderCurrent();
        sceneManager_.updateCurrent(deltaTime);

        surface_->Present();
    }

    void GLRenderer::Cleanup()
    {
        if (isCleanedUp_) return;

        isCleanedUp_ = true;
    }

    void GLRenderer::SetScene(std::unique_ptr<IRenderScene> scene)
    {
        if (!isInitialized_) {
            throw std::logic_error("GLRenderer::SetScene called before Initialize()");
        }

        if (!scene) {
            throw std::invalid_argument("GLRenderer::SetScene received null scene");
        }

        sceneManager_.setScene(std::move(scene));
    }

}
