#pragma once

#include <GLRenderer/Interface/IRenderer.hpp>
#include <GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>
#include <GLRenderer/OpenGL/Types/GLRenderState.hpp>
#include <GLRenderer/OpenGL/GLSceneManager.hpp>
#include <GLRenderer/Window/Interface/IRenderSurface.hpp>
#include <memory>

namespace GLRenderer
{
    class GLRenderer : public IRenderer
    {
    public:
        GLRenderer() = default;
        ~GLRenderer() override;

        void Initialize(IRendererContextDesc& contextDesc, IRenderSurface& surface) override;
        void SetScene(std::unique_ptr<IRenderScene> scene);
        void SetRenderSurface(IRenderSurface* surface) override;
        void SetWindow(Window::IWindow& window);
        void RenderFrame() override;
        void Cleanup() override;

    private:
        void InitializeDefaults();
        void InitializeOpenGL(IRendererContextDesc& contextDesc);
    //    void LoadInitialScene();
        bool isInitialized_ = false;
        bool isCleanedUp_ = false;

        float deltaTime = 0.0f;
        double lastTime_ = 0.0;  // store time of previous frame

        GLRendererContextDesc contextDesc_{};
		GLRenderState state_{};
        GLSceneManager sceneManager_;
		//HelloTriangle helloTriangleScene_;
       // TexturedQuad texturedTriangleScene_;
       // std::unique_ptr<LightingCubes>  lightCubes_;
       // std::unique_ptr<Quad3D> quad3DScene_;
       // std::unique_ptr<ModelLoading> modelLoadingScene_;

        Window::IWindow* window_ = nullptr; // Non-owning pointer
        IRenderSurface* surface_;
    };
}
