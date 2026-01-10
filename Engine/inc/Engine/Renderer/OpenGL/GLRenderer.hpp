#pragma once

// Interface 
#include <Renderer/Interface/IRenderer.hpp>
#include <Platform/RenderSurface/Interface/IRenderSurface.hpp>
#include <Renderer/Interface/Types/IRendererContextDesc.hpp>
#include <Renderer/Interface/IRenderScene.hpp>

// OpenGL specific
#include <Renderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <Renderer/OpenGL/Types/GLRenderState.hpp>

// STL
#include <memory>

namespace Renderer {
    namespace OpenGL {

        class GLRenderer : public ::Renderer::IRenderer
        {
        public:
            GLRenderer();
            ~GLRenderer() override;

            void Initialize(IRendererContextDesc& contextDesc, Platform::RenderSurface::IRenderSurface& surface) override;
            void SetRenderSurface(Platform::RenderSurface::IRenderSurface* surface) override;
            std::unique_ptr<IRendererContextDesc> CreateDefaultContextDesc() override;
            void RenderScene(const IRenderScene& renderScene) override;
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

            Platform::RenderSurface::IRenderSurface* surface_;
        };
    }
}
