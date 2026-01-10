#pragma once

#include <glm/glm.hpp>
#include <Renderer/Interface/Types/CameraCommand.hpp>

namespace Renderer {

    class ICamera {
    public:
        virtual ~ICamera() = default;

        virtual void ApplyCommand(const CameraCommand& command, float deltaTime) = 0;
        virtual glm::mat4 GetViewMatrix() const = 0;
        virtual float GetFOV() const = 0;
    };

}
