#pragma once

#include <Renderer/Interface/Types/IRendererContextDesc.hpp>
#include <Platform/RenderSurface/Interface/IRenderSurface.hpp>

namespace Renderer {

    class IRenderScene;

    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        // Initialize with a context and window
        virtual void Initialize(IRendererContextDesc& contextDesc, Platform::RenderSurface::IRenderSurface& window) = 0;

        // Render an entire scene
        virtual void RenderScene(const IRenderScene& renderScene) = 0;

        // Optional: swap render targets / surfaces
        virtual void SetRenderSurface(Platform::RenderSurface::IRenderSurface* surface) = 0;

        // Cleanup GPU resources
        virtual void Cleanup() = 0;
    };

} // namespace Renderer
