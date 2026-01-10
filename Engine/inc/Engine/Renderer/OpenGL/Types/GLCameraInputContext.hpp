#pragma once
#include <Renderer/OpenGL/GLCamera.hpp>

namespace Renderer {
    namespace OpenGL {
        struct GLCameraInputContext {
            GLCamera* camera = nullptr;

            float lastX = 0.0f;
            float lastY = 0.0f;
            bool firstMouse = true;
            bool ignoreNextMouseDelta = false;
        };
    }
}

