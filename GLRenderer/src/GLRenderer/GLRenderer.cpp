#include <GLRenderer/OpenGL/GLRenderer.hpp>
#include <GLRenderer/OpenGL/OpenGLInitializer.hpp>
#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <stdexcept>
#include <typeinfo>

namespace GLRenderer
{
    void GLRenderer::Initialize(IRendererContextDesc& contextDesc, Window::IWindow& window)
    {
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
    }

    void GLRenderer::RenderFrame()
    {
        if (!isInitialized_) {
            throw std::runtime_error("RenderFrame called before GLRenderer was initialized.");
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLRenderer::Cleanup()
    {
        if (isCleanedUp_) return;

        // TODO: OpenGL cleanup logic (e.g., delete buffers, shaders, etc.)
        // For example: OpenGLInitializer::Shutdown();

        isCleanedUp_ = true;
    }
}
