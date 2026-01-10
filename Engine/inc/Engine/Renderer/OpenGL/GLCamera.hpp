#pragma once

#include <Renderer/Interface/ICamera.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
    namespace OpenGL {
        struct CameraAttributes {
            glm::vec3 position{ 0.0f };
            glm::vec3 targetPosition{ 0.0f };

            float yaw = -90.0f;
            float pitch = 0.0f;

            float moveSpeed = 2.5f;
            float lookSensitivity = 0.1f;
            float fov = 45.0f;

            glm::vec3 front{ 0.0f, 0.0f, -1.0f };
            glm::vec3 up{ 0.0f, 1.0f, 0.0f };
            glm::vec3 right{ 1.0f, 0.0f, 0.0f };
            glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };
        };

        class GLCamera final : public Renderer::ICamera {
        public:
            GLCamera(
                glm::vec3 position = { 0.0f, 0.0f, 0.0f },
                glm::vec3 up = { 0.0f, 1.0f, 0.0f },
                float yaw = -90.0f,
                float pitch = 0.0f
            );

            // ICamera
            void ApplyCommand(const CameraCommand& command, float deltaTime) override;
            glm::mat4 GetViewMatrix() const override;
            float GetFOV() const override;

            void UpdateVectors() { updateCameraVectors(); }

            CameraAttributes& GetAttributes() { return attributes_; }
            const CameraAttributes& GetAttributes() const { return attributes_; }

        private:
            CameraAttributes attributes_;

            void updateCameraVectors();
        };
    }
}
