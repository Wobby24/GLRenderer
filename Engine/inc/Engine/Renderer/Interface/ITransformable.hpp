#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Renderer {

    struct ITransformable {
        // Non-const reference accessors (for in-place modification)
        virtual glm::vec3& GetPosition() = 0;
        virtual glm::quat& GetRotation() = 0;
        virtual glm::vec3& GetScale() = 0;

        // Const reference accessors (for read-only access)
        virtual const glm::vec3& GetPosition() const = 0;
        virtual const glm::quat& GetRotation() const = 0;
        virtual const glm::vec3& GetScale() const = 0;

        // Setters
        virtual void SetPosition(const glm::vec3&) = 0;
        virtual void SetRotation(const glm::quat&) = 0;
        virtual void SetScale(const glm::vec3&) = 0;

        // Matrix getter
        virtual glm::mat4 GetTransformMatrix() const = 0;

        virtual ~ITransformable() = default;
    };

}
