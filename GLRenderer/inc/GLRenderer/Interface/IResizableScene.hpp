#pragma once
#include <GLRenderer/Window/Interface/IWindow.hpp>
#include "IRenderScene.hpp"

namespace GLRenderer {
    class IResizableScene :public IRenderScene {
    public:
        virtual ~IResizableScene() = default;
        virtual void OnWindowResize(int width, int height) = 0;
        virtual void SetWindow(Window::IWindow& window) = 0;
    };
}
