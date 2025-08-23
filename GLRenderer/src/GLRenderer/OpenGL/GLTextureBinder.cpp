#include "GLRenderer/OpenGL/GLTextureBinder.hpp"
#include <glad/glad.h> // For glBindTexture, etc.

namespace GLRenderer {

    GLTextureBinder::GLTextureBinder() {
        Reset();
    }

    bool GLTextureBinder::BindTexture(const std::shared_ptr<GLTexture2D>& texture, int unit) {
        if (unit < 0 || unit >= MAX_TEXTURE_UNITS) {
            // Invalid texture unit, optionally log error
            return false;
        }
        if (!texture) {
            // Null texture, unbind texture at this unit if needed
            if (boundTextures_[unit] != 0) {
                glActiveTexture(GL_TEXTURE0 + unit);
                glBindTexture(GL_TEXTURE_2D, 0);
                boundTextures_[unit] = 0;
                return true;
            }
            return false;
        }

        unsigned int texID = texture->getID();
        if (boundTextures_[unit] == texID) {
            // Already bound, no action needed
            return false;
        }

        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texID);
        boundTextures_[unit] = texID;
        return true;
    }

    void GLTextureBinder::Reset() {
        for (int i = 0; i < MAX_TEXTURE_UNITS; ++i) {
            boundTextures_[i] = 0;
        }
    }

} // namespace GLRenderer
