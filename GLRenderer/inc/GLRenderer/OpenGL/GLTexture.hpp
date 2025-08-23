#pragma once

#include <glad/glad.h>
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

    // Add texture types enum
    enum class TextureType {
        DIFFUSE,
        SPECULAR,
        EMISSION,
        NORMAL,
        UNKNOWN
    };

    class GLTexture2D : public ITexture {
    public:
        // Add optional texture type param, default UNKNOWN
        GLTexture2D(const std::string& path, TextureType type = TextureType::UNKNOWN);
        ~GLTexture2D() override;

        void loadTexture() override;

        void setWrap(GLTextureWrap s, GLTextureWrap t);
        void setFilter(GLTextureFilter minFilter, GLTextureFilter magFilter);

        void bind(unsigned int slot = 0) const;
        void unbind() const;

        void setVertTexFlip(bool toggle) { isVertFlipped_ = toggle; }
        bool getVertTexFlip() { return isVertFlipped_; }

        bool isCleanedUp() const { return isCleaned_; }
        bool isLoaded() const { return isLoaded_; }
        const std::string& getFilePath() const { return filePath_; }

        void cleanup();
        void reload();

        unsigned int getID() const { return id_; }

        // Texture type getter
        TextureType getType() const { return type_; }
        // Texture type setter
        void setType(TextureType type) { type_ = type; }

        // Optional: get string version of type for convenience
        std::string getTypeString() const;

    private:
        std::string filePath_;
        bool isLoaded_ = false;
        bool isCleaned_ = false;
        bool isVertFlipped_ = false;
        unsigned int id_ = 0;

        GLTextureWrap wrapS_ = GLTextureWrap::REPEAT;
        GLTextureWrap wrapT_ = GLTextureWrap::REPEAT;
        GLTextureFilter minFilter_ = GLTextureFilter::LINEAR;
        GLTextureFilter magFilter_ = GLTextureFilter::LINEAR;

        TextureType type_ = TextureType::UNKNOWN;
    };

}
