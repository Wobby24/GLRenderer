#pragma once

#include <glm/glm.hpp>

#define MAX_BONE_INFLUENCE 4

namespace Renderer {
    struct Vertex {
        glm::vec3 position{ 0.0f };
        glm::vec3 normal{ 0.0f };
        glm::vec2 texCoords{ 0.0f };
        // tangent
        glm::vec3 tangent{ 0.0f };
        // bitangent
        glm::vec3 bitangent{ 0.0f };
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE]{};
        float m_Weights[MAX_BONE_INFLUENCE]{};

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

        Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex)
            : position(pos), texCoords(tex), normal(norm) {
        }
    };
}

