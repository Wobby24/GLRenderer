#include <glad/glad.h>
#include <Renderer/OpenGL/Types/GLRenderState.hpp>

namespace Renderer {
	namespace OpenGL {
		void GLRenderState::ApplyState() {
			//depth testing enabled
			if (depthTestEnabled) glEnable(GL_DEPTH_TEST);
			else glDisable(GL_DEPTH_TEST);
			//switch case on the depth function
			switch (depthFunc) {
			case GL_NEVER:
				glDepthFunc(GL_NEVER);
				break;
			case GL_LESS:
				glDepthFunc(GL_LESS);
				break;
			case GL_LEQUAL:
				glDepthFunc(GL_LEQUAL);
				break;
			case GL_EQUAL:
				glDepthFunc(GL_EQUAL);
				break;
			case GL_NOTEQUAL:
				glDepthFunc(GL_NOTEQUAL);
				break;
			case GL_GREATER:
				glDepthFunc(GL_GREATER);
				break;
			case GL_GEQUAL:
				glDepthFunc(GL_GEQUAL);
				break;
			case GL_ALWAYS:
				glDepthFunc(GL_ALWAYS);
				break;
			}
			//apply the clear color 
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		}
	}
}