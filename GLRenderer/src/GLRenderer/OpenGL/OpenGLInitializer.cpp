#include <glad/glad.h>
#include "GLRenderer/OpenGL/OpenGLInitializer.hpp"
#include "GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

namespace GLRenderer {

	bool OpenGLInitializer::initialized = false;

	bool OpenGLInitializer::Initialize(GLRendererContextDesc& contextDesc) {
		if (initialized)
			return true;

		if (!gladLoadGL()) {
			std::cerr << "Failed to initialize GLAD." << std::endl;
			return false;
		}

		CheckOpenGLVersion(contextDesc.majorVersion, contextDesc.minorVersion);

		initialized = true;
		return true;
	}

	void OpenGLInitializer::Shutdown() {
		initialized = false;
		// No OpenGL-specific shutdown needed, unless you want to clear resources
	}

	void OpenGLInitializer::CheckOpenGLVersion(int major, int minor) {
		const int requiredMajor = 3;
		const int requiredMinor = 3;

		if (major < requiredMajor || (major == requiredMajor && minor < requiredMinor)) {
			throw std::runtime_error(
				"OpenGL version " + std::to_string(major) + "." + std::to_string(minor) +
				" is not supported. Requires at least " +
				std::to_string(requiredMajor) + "." + std::to_string(requiredMinor)
			);
		}
	}

} // namespace GLRenderer
