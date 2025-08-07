#pragma once

#include <glm/glm.hpp>

namespace GLRenderer {
    class ICamera {
    public:
        virtual ~ICamera() = default;   // virtual destructor with default implementation
        virtual glm::mat4 getViewMatrix() const = 0;  // pure virtual: must be implemented by derived classes
    };
}

