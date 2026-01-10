#pragma once

#include <glm/glm.hpp>

namespace Renderer {

    struct CameraCommand {
        glm::vec3 move{ 0.0f };   // x = right, y = up, z = forward (camera space)
        glm::vec2 look{ 0.0f };   // x = yaw delta, y = pitch delta
        float zoomDelta = 0.0f;
        bool boost = false;
    };

}
