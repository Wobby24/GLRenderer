#pragma once

#include <GLRenderer/OpenGL/Types/GLRendererContextDesc.hpp>

namespace GLRenderer
{
	class OpenGLInitializer
	{
	public:
		//init function to initialize OpenGL with the given major and minor version numbers
		static bool Initialize(GLRendererContextDesc& contextDesc);
		// shutdown function to clean up OpenGL resources
		static void Shutdown();
		
		//bool to tell if OpenGL has been initialized
		static bool IsInitialized() { return initialized; }
	private:
		//the bool we return for IsInitialized()
		static bool initialized;		
		/* Basic function to detect if OpenGL version is valid */
		static void CheckOpenGLVersion(int major, int minor); // Now local to the class
	};
} // namespace GLRenderer
