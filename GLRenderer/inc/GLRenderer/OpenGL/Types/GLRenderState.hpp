#pragma once

#include <glm/glm.hpp>

namespace GLRenderer {
    struct GLRenderState {
        glm::vec4 clearColor;
        bool depthTestEnabled; //we will use this later
        //GLenum cullMode;     // GL_BACK, GL_FRONT, etc. //same for this 
        //GLenum depthFunc;    // GL_LESS, GL_EQUAL, etc. //and this 
        //bool wireframeMode; //and this
        // Add more state as needed
    };

}
