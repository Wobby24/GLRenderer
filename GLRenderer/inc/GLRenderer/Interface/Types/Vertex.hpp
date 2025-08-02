#pragma once

#include <glm/glm.hpp>

namespace GLRenderer {
    struct Vertex {
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 color{ 1.0f, 1.0f, 1.0f };
        glm::vec2 texCoords{ 0.0f, 0.0f };

        Vertex() = default;

        Vertex(const glm::vec3& pos, const glm::vec3& col, const glm::vec2& tex = glm::vec2{ 0.0f, 0.0f })
            : position(pos), color(col), texCoords(tex) {
        }
    };

}

