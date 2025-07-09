#pragma once

#include <GLRenderer/Interface/IRenderer.hpp>
#include <GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp>
#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>
#include <memory>
#include <GLRenderer/Window/Window.hpp>

namespace GLRenderer
{
    class GLRenderer : public IRenderer
    {
    public:
        GLRenderer() = default;
        ~GLRenderer() override = default;

        void Initialize(IRendererContextDesc& contextDesc, Window::IWindow& window) override;
        void RenderFrame() override;
        void Cleanup() override;

    private:
        bool isInitialized_ = false;
        bool isCleanedUp_ = false;

        GLRendererContextDesc contextDesc_{};
        Window::IWindow* window_ = nullptr; // Non-owning pointer
    };
}
