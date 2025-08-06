#include <GLRenderer/OpenGL/GLRenderer.hpp>
#include <GLRenderer/OpenGL/OpenGLInitializer.hpp>
#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRenderState.hpp>
#include <GLFW/glfw3.h>  // for glfwGetTime()
#include <stdexcept>

namespace GLRenderer
{

   // GLRenderer::GLRenderer() : isInitialized_(false), isCleanedUp_(false) {}

    GLRenderer::~GLRenderer() {
        if (isCleanedUp_) return;
        Cleanup();
    }

    void GLRenderer::Initialize(IRendererContextDesc& contextDesc, Window::IWindow& window)
    {
        InitializeDefaults();
        window_ = &window;
        InitializeOpenGL(contextDesc);
        isInitialized_ = true;
        state_.ApplyState();
        sceneManager_.init();

        InitializeScenes(); // <-- put this here instead
        lastTime_ = glfwGetTime();
    }

    void GLRenderer::InitializeDefaults()
    {
        // Set default render state
        state_.clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f); // Default clear color
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

    void GLRenderer::InitializeScenes() {
        auto scene = std::make_unique<Quad3D>();

        // Save the handle returned by addScene
        SceneHandle handle = sceneManager_.addScene(std::move(scene));

        // Set that handle as the current scene
        sceneManager_.setCurrentScene(handle);
        sceneManager_.initCurrent();
    }

    void GLRenderer::RenderFrame()
    {
        if (!isInitialized_) {
            throw std::runtime_error("RenderFrame called before GLRenderer was initialized.");
        }

        if (isCleanedUp_) {
            throw std::runtime_error("RenderFrame called after clean-up!");
        }

        double currentTime = glfwGetTime();
        deltaTime = static_cast<float>(currentTime - lastTime_);
        lastTime_ = currentTime;

        sceneManager_.resizeCurrent(window_->GetSize().x, window_->GetSize().y);
        sceneManager_.renderCurrent();
        sceneManager_.updateCurrent(deltaTime);
    }

    void GLRenderer::Cleanup()
    {
        if (isCleanedUp_) return;

        sceneManager_.cleanup();

        isCleanedUp_ = true;
    }
}
