#pragma once

#include <glm/glm.hpp>

namespace GLRenderer {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;

        Vertex(float x, float y, float z, float r, float g, float b)
            : position{ x, y, z }, color{ r, g, b } {
        }

        Vertex(const glm::vec3& pos, const glm::vec3& col)
            : position(pos), color(col) {
        }
    };
}
