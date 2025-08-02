#include <GLRenderer/OpenGL/GLRenderer.hpp>
#include <GLRenderer/OpenGL/OpenGLInitializer.hpp>
#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRenderState.hpp>
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
		texturedTriangleScene_.Init();
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

        //set RGBA clear color
        glClearColor(state_.clearColor.r, state_.clearColor.g, state_.clearColor.b, state_.clearColor.a);
        //clear depth buffer
        glClear(GL_COLOR_BUFFER_BIT);

        texturedTriangleScene_.Render();
		//for the helloTriangle scene, we can render it directly, there really is no updates unless we need to switch wireframe, but thats not something we are doing
    }

    void GLRenderer::Cleanup()
    {
        if (isCleanedUp_) return;

        // TODO: OpenGL cleanup logic (e.g., delete buffers, shaders, etc.)
        // For example: OpenGLInitializer::Shutdown();
        texturedTriangleScene_.Cleanup();

        isCleanedUp_ = true;
    }
}
