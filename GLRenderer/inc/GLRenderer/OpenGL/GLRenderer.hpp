#pragma once

#include <GLRenderer/Interface/IRenderer.hpp>
#include <GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRenderState.hpp>
#include <GLRenderer/OpenGL/Scenes/C1/HelloTriangle.hpp>
#include <GLRenderer/OpenGL/Scenes/C1/TexturedQuad.hpp>
#include <GLRenderer/OpenGL/Scenes/C1/Quad3D.hpp>
#include <GLRenderer/OpenGL/GLSceneManager.hpp>
#include <memory>
#include <GLRenderer/Window/Window.hpp>

namespace GLRenderer
{
    class GLRenderer : public IRenderer
    {
    public:
        GLRenderer() = default;
        ~GLRenderer() override;

        void Initialize(IRendererContextDesc& contextDesc, Window::IWindow& window) override;
        void RenderFrame() override;
        void Cleanup() override;

    private:
        void InitializeDefaults();
        void InitializeOpenGL(IRendererContextDesc& contextDesc);
        void InitializeScenes();
        bool isInitialized_ = false;
        bool isCleanedUp_ = false;

        float deltaTime = 0.0f;
        double lastTime_ = 0.0;  // store time of previous frame

        GLRendererContextDesc contextDesc_{};
		GLRenderState state_{};
        GLSceneManager sceneManager_;
		HelloTriangle helloTriangleScene_;
        TexturedQuad texturedTriangleScene_;
        std::unique_ptr<Quad3D> quad3DScene_;

        Window::IWindow* window_ = nullptr; // Non-owning pointer
    };
}
