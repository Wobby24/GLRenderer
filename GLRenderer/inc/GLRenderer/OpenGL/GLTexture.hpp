#pragma once

#include <GLRenderer/Interface/ITexture.hpp>
#include <string>

namespace GLRenderer {
    enum class GLTextureWrap : GLenum {
        REPEAT = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };

    enum class GLTextureFilter : GLenum {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR
    };

    class GLTexture2D : public ITexture {
    public:
        GLTexture2D(const std::string& path);
        ~GLTexture2D() override;

        void loadTexture() override;

        void setWrap(GLTextureWrap s, GLTextureWrap t);
        void setFilter(GLTextureFilter minFilter, GLTextureFilter magFilter);

        void bind(unsigned int slot = 0) const;
        void unbind() const;

        bool isCleanedUp() const { return isCleaned_; }
        bool isLoaded() const { return isLoaded_; }

        void cleanup();
        void reload();

        unsigned int getID() const { return id_;  }

    private:
        bool isLoaded_ = false;
        bool isCleaned_ = false;
        unsigned int id_ = 0;
        std::string filePath_;

        GLTextureWrap wrapS_ = GLTextureWrap::REPEAT;
        GLTextureWrap wrapT_ = GLTextureWrap::REPEAT;
        GLTextureFilter minFilter_ = GLTextureFilter::LINEAR;
        GLTextureFilter magFilter_ = GLTextureFilter::LINEAR;
    };
}