#include <Renderer/OpenGL/GLCamera.hpp>
#include <Utils/Math/Clamp.hpp>
#include <Utils/Math/Lerp.hpp>

#include <algorithm>
#include <cmath>

namespace Renderer {
    namespace OpenGL {
        GLCamera::GLCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
            attributes_.position = position;
            attributes_.targetPosition = position;
            attributes_.worldUp = up;
            attributes_.yaw = yaw;
            attributes_.pitch = pitch;
            updateCameraVectors();
        }

        void GLCamera::ApplyCommand(const CameraCommand& cmd, float dt) {
            const float speed = attributes_.moveSpeed * (cmd.boost ? 2.0f : 1.0f);

            // Movement (camera-local space)
            attributes_.targetPosition +=
                attributes_.front * cmd.move.z * speed * dt +
                attributes_.right * cmd.move.x * speed * dt +
                attributes_.up * cmd.move.y * speed * dt;

            // Rotation
            attributes_.yaw += cmd.look.x * attributes_.lookSensitivity;
            attributes_.pitch += cmd.look.y * attributes_.lookSensitivity;

            attributes_.pitch = Utils::Math::clamp(attributes_.pitch, -89.0f, 89.0f);

            // Zoom
            attributes_.fov = Utils::Math::clamp(
                attributes_.fov - cmd.zoomDelta,
                1.0f,
                90.0f
            );

            // Smooth movement
            const float t = std::min(dt * 10.0f, 1.0f);
            attributes_.position = Utils::Math::lerp(attributes_.position, attributes_.targetPosition, t);

            updateCameraVectors();
        }

        glm::mat4 GLCamera::GetViewMatrix() const {
            return glm::lookAt(
                attributes_.position,
                attributes_.position + attributes_.front,
                attributes_.up
            );
        }

        float GLCamera::GetFOV() const {
            return attributes_.fov;
        }

        void GLCamera::updateCameraVectors() {
            glm::vec3 front;
            front.x = cos(glm::radians(attributes_.yaw)) * cos(glm::radians(attributes_.pitch));
            front.y = sin(glm::radians(attributes_.pitch));
            front.z = sin(glm::radians(attributes_.yaw)) * cos(glm::radians(attributes_.pitch));

            attributes_.front = glm::normalize(front);
            attributes_.right = glm::normalize(glm::cross(attributes_.front, attributes_.worldUp));
            attributes_.up = glm::normalize(glm::cross(attributes_.right, attributes_.front));
        }
    }
}
