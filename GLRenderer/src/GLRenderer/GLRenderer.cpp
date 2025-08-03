#include <GLRenderer/OpenGL/GLRenderer.hpp>
#include <GLRenderer/OpenGL/OpenGLInitializer.hpp>
#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRenderState.hpp>
#include <GLFW/glfw3.h>  // for glfwGetTime()
#include <stdexcept>

namespace GLRenderer
{
    void GLRenderer::Initialize(IRendererContextDesc& contextDesc, Window::IWindow& window)
    {
        InitializeDefaults();
        window_ = &window;
        InitializeOpenGL(contextDesc);
        isInitialized_ = true;
        state_.ApplyState();

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
        quad3DScene_.Init(); // Temp — to be replaced by sceneManager_.InitAll();
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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        int width, height;
        // 1. Get the current window size
        window_->GetSize(width, height);

        // 2. Tell the scene about the new window size
        quad3DScene_.onWindowResize(width, height);

        // 3. Update the scene with correct size info
        quad3DScene_.Update(deltaTime);
        
        // 4. Render
        quad3DScene_.Render();
    }

    void GLRenderer::Cleanup()
    {
        if (isCleanedUp_) return;

        quad3DScene_.Cleanup();

        isCleanedUp_ = true;
    }
}
