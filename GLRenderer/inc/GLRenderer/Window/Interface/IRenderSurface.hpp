#pragma once

#include <glm/glm.hpp>

namespace GLRenderer {
    class IRenderSurface {
    public:
        virtual ~IRenderSurface() = default;

        virtual glm::ivec2 GetSize() const = 0;
        virtual void Bind() = 0;   // make framebuffer current
        virtual void Present() = 0;   // end frame
    };
}