#pragma once 

#include <glm/glm.hpp>

namespace GLRenderer {
    struct ITransformable {
        virtual glm::vec3 GetPosition() const = 0;
        virtual glm::quat GetRotation() const = 0;
        virtual glm::vec3 GetScale() const = 0;

        virtual void SetPosition(const glm::vec3&) = 0;
        virtual void SetRotation(const glm::quat&) = 0;
        virtual void SetScale(const glm::vec3&) = 0;

        virtual glm::mat4 GetTransformMatrix() const = 0;

        virtual ~ITransformable() = default;
    };
}
