#pragma once
#include <GLRenderer/OpenGL/GLCamera.hpp>

namespace GLRenderer {
    struct GLCameraInputContext {
        GLCamera* camera = nullptr;

        float lastX = 0.0f;
        float lastY = 0.0f;
        bool firstMouse = true;
    };
}

