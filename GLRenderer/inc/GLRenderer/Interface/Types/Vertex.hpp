#pragma once

#include <glm/glm.hpp>

namespace GLRenderer {
    struct Vertex {
        glm::vec3 position{ 0.0f };
        glm::vec2 texCoords{ 0.0f };
        glm::vec3 normal{ 0.0f };

        Vertex() = default;

        Vertex(const glm::vec3& pos)
            : position(pos) {
        }

        Vertex(const glm::vec3& pos, const glm::vec2& tex)
            : position(pos), texCoords(tex) {
        }

        Vertex(const glm::vec3& pos, const glm::vec3& norm)
            : position(pos), normal(norm) {
        }

        Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& norm) 
            : position(pos), texCoords(tex), normal(norm) {
        }

    };
}

