#include <GLRenderer/Utils/getAbsolutePath.hpp>
#include <glad/glad.h>
///surpress warnings for stb_image
#ifdef _MSC_VER
#pragma warning(push, 0)
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#endif

#include "stb_image/stb_image.h"

#ifdef _MSC_VER
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <stdexcept>

namespace GLRenderer {
	GLTexture2D::GLTexture2D(const std::string& path, TextureType type)
		: filePath_(path),
		isLoaded_(false),
		isCleaned_(false),
		id_(0),
		wrapS_(GLTextureWrap::REPEAT),
		wrapT_(GLTextureWrap::REPEAT),
		minFilter_(GLTextureFilter::LINEAR),
		magFilter_(GLTextureFilter::LINEAR),
		type_(type)
	{}

	GLTexture2D::~GLTexture2D() {
		cleanup();
	}

	void GLTexture2D::cleanup() {
		if (isCleaned_) return; // Already cleaned up

		if (id_ != 0) {
			glDeleteTextures(1, &id_);
			id_ = 0;
		}

		isLoaded_ = false;
		isCleaned_ = true;
	}

	void GLTexture2D::setWrap(GLTextureWrap s, GLTextureWrap t) {
		wrapS_ = s;
		wrapT_ = t;
	}

	void GLTexture2D::setFilter(GLTextureFilter minFilter, GLTextureFilter magFilter) {
		minFilter_ = minFilter;
		magFilter_ = magFilter;
	}

	std::string GLTexture2D::getTypeString() const {
		switch (type_) {
		case TextureType::DIFFUSE: return "diffuse";
		case TextureType::SPECULAR: return "specular";
		case TextureType::EMISSION: return "emission";
		case TextureType::NORMAL: return "normal";
		default: return "unknown";
		}
	}

	void GLTexture2D::bind(unsigned int slot) const {
		glActiveTexture(GL_TEXTURE0 + slot); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, id_);
	}

	void GLTexture2D::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::loadTexture() {
		stbi_set_flip_vertically_on_load(isVertFlipped_);

		// 1. Extract and load data with stb_image
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath_.c_str(), &width, &height, &nrChannels, 0);

		// 1a. Check for errors
		if (!data) {
			throw std::runtime_error("Texture data invalid! Could not load the texture!");
		}

		// 2. Generate texture ID and bind it
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);

		// 3. Set texture parameters (wrap/filter), this is safe since the enums are equal to the actual gl enums
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrapS_));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrapT_));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minFilter_));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(magFilter_));

		// 4. Determine the image format (color channels)
		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		// 5. Upload the texture data to the gpu 
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		// 6. Generate mip map
		glGenerateMipmap(GL_TEXTURE_2D);

		// 7. Cleanup
		stbi_image_free(data);

		// 8. update state
		isLoaded_ = true;
		isCleaned_ = false;

		// 9. unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::reload() {
		cleanup();
		loadTexture();
	}

	void GLTexture2D::loadTexture(const std::string& baseDir) {
		baseDir_ = baseDir; // store it if needed later

		// Resolve the absolute path using your helper
		std::string absPath = GLRenderer::getAbsoluteResourcePath(filePath_, baseDir_);

		std::cout << "[Debug] absoluteInput: \"" << absPath << "\"\n";

		stbi_set_flip_vertically_on_load(isVertFlipped_);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(absPath.c_str(), &width, &height, &nrChannels, 0);

		if (!data) {
			throw std::runtime_error("Texture data invalid! Could not load the texture at: " + absPath);
		}

		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrapS_));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrapT_));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minFilter_));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(magFilter_));

		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		isLoaded_ = true;
		isCleaned_ = false;

		glBindTexture(GL_TEXTURE_2D, 0);
	}

}