#include <Renderer/OpenGL/GLRenderer.hpp>
#include <Renderer/OpenGL/OpenGLInitializer.hpp>
#include <Renderer/Interface/Types/IRendererContextDesc.hpp>
#include <Renderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <Renderer/OpenGL/Types/GLRenderState.hpp>
#include <Platform/RenderSurface/GLFW_RenderSurface.hpp>
#include <Platform/RenderSurface/Interface/IRenderSurface.hpp>
#include <Renderer/Interface/IRenderScene.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>  // for glfwGetTime()
#include <stdexcept>

namespace Renderer {
    namespace OpenGL {
        GLRenderer::~GLRenderer() {
            if (isCleanedUp_) return;
            Cleanup();
        }

        void GLRenderer::Initialize(IRendererContextDesc& contextDesc, Platform::RenderSurface::IRenderSurface& surface)
        {
            surface_ = &surface; // store pointer

            InitializeDefaults();
            InitializeOpenGL(contextDesc);
            isInitialized_ = true;
            state_.ApplyState();

            lastTime_ = glfwGetTime();
        }

        void GLRenderer::SetRenderSurface(Platform::RenderSurface::IRenderSurface* surface) {
            if (!surface) throw std::runtime_error("Surface cannot be null");
            surface_ = surface;
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

        void GLRenderer::RenderScene(const IRenderScene& renderScene)
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

            // Render the scene, nothing else
            renderScene.Render();

            surface_->Present();
        }

        void GLRenderer::Cleanup()
        {
            if (isCleanedUp_) return;

            isCleanedUp_ = true;
        }
    }
}
