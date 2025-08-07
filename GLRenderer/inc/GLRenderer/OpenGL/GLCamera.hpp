#pragma once

#include <GLRenderer/Interface/ICamera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GLRenderer {
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    struct CameraAttributes {
        float yaw = -90.0f;
        float pitch = 0.0f;
        float movementSpeed = 2.5f;
        float mouseSensitivity = 0.1f;
        float zoom = 45.0f;
        double alpha = 0.1;

        glm::vec3 position = glm::vec3(0.0);
        glm::vec3 targetPosition = glm::vec3(0.0);
        glm::vec3 front = glm::vec3(0.0, 0.0, -1.0);
        glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
        glm::vec3 right = glm::vec3(1.0, 0.0, 0.0);
        glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0);
    };

    class GLCamera : public ICamera {
    public:
        // Constructor using default attribute values
        GLCamera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = -90.0f,
            float pitch = 0.0f
        );

        GLCamera(float posX, float posY, float posZ,
            float upX, float upY, float upZ,
            float yaw, float pitch);

        ~GLCamera() override = default;

        glm::mat4 getViewMatrix() const override;
        void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void processKeyboard(Camera_Movement direction, float deltaTime);
        void processMouseScroll(float yoffset);
        void updatePosition(float deltaTime);
        CameraAttributes& getAttributes() { return attributes_; }
    private:
        CameraAttributes attributes_;

        // Recalculate front/right/up vectors from yaw and pitch
        void updateCameraVectors();
        glm::vec3 lerp(const glm::vec3& start, const glm::vec3& end, float t);
    };

}

