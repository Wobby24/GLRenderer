#pragma once

#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>
#include <GLRenderer/Window/Interface/IRenderSurface.hpp>

namespace GLRenderer
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        // Takes a pointer to a generic context description
        virtual void Initialize(IRendererContextDesc& contextDesc, ::GLRenderer::IRenderSurface& window) = 0;
        virtual void RenderFrame() = 0;
        virtual void SetRenderSurface(IRenderSurface* surface) = 0;
        virtual void Cleanup() = 0;
    };
}
