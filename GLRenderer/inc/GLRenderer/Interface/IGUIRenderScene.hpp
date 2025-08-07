#pragma once

#include "IRenderScene.hpp"

namespace GLRenderer {

    class IGUIRenderScene : public virtual IRenderScene {
    public:
        virtual ~IGUIRenderScene() = default;

        // Render ImGui or any UI components here
        virtual void RenderGUI() = 0;
    };

}
