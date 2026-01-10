#pragma once

#include <glad/glad.h>  // Required for GLenum and OpenGL constants
#include <glm/glm.hpp>

namespace Renderer {
    namespace OpenGL {

        struct GLRenderState {
            glm::vec4 clearColor;
            bool depthTestEnabled; //we will use this later
            GLenum depthFunc;    // GL_LESS, GL_EQUAL, etc. //and this 
            bool wireframeMode; //and this
            void ApplyState();// function to apply the state
        };

    }
}
