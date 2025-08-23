#pragma once 

#include <GLRenderer/Interface/ITransformable.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GLRenderer {
    class Transformable : public ITransformable {
    public:
        glm::vec3 position{ 0.0f };
        glm::quat rotation{ 1.0, 0.0, 0.0, 0.0 };
        glm::vec3 scale{ 1.0f };

        glm::vec3 GetPosition() const override { return position; }
        glm::quat GetRotation() const override { return rotation; }
        glm::vec3 GetScale() const override { return scale; }

        void SetPosition(const glm::vec3& pos) override { position = pos; }
        void SetRotation(const glm::quat& rot) override { rotation = rot; }
        void SetScale(const glm::vec3& scl) override { scale = scl; }

        glm::mat4 GetTransformMatrix() const override {
            glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 R = glm::mat4_cast(rotation);
            glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
            return T * R * S;
        }
    };
}
