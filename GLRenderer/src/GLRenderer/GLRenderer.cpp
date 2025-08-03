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
        // Save pointer for later use (non-owning)
        window_ = &window;
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
        isInitialized_ = true;

        // Initialize Scene(s)
		quad3DScene_.Init();

        // Initialize lastTime_ here AFTER context creation and GLFW initialized
        lastTime_ = glfwGetTime();
    }

    void GLRenderer::InitializeDefaults()
    {
        // Set default render state
        state_.clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f); // Default clear color
        // Additional defaults can be set here as needed
    }

    void GLRenderer::RenderFrame()
    {
        if (!isInitialized_) {
            throw std::runtime_error("RenderFrame called before GLRenderer was initialized.");
        }

        double currentTime = glfwGetTime();
        deltaTime = static_cast<float>(currentTime - lastTime_);
        lastTime_ = currentTime;

        glClearColor(state_.clearColor.r, state_.clearColor.g, state_.clearColor.b, state_.clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        quad3DScene_.Update(deltaTime);
        quad3DScene_.Render();
    }

    void GLRenderer::Cleanup()
    {
        if (isCleanedUp_) return;

        // TODO: OpenGL cleanup logic (e.g., delete buffers, shaders, etc.)
        // For example: OpenGLInitializer::Shutdown();
        quad3DScene_.Cleanup();

        isCleanedUp_ = true;
    }
}
