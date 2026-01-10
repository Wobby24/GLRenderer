#include <Platform/RenderSurface/GLFW_RenderSurface.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Platform {
    namespace RenderSurface {

        GLFW_RenderSurface::GLFW_RenderSurface(GLFWwindow* window)
            : window_(window)
        {
            if (!window_) {
                throw std::runtime_error("GLFWRenderSurface: GLFWwindow is null");
            }
        }

        glm::ivec2 GLFW_RenderSurface::GetSize() const
        {
            int w = 0, h = 0;
            glfwGetFramebufferSize(window_, &w, &h);
            return { w, h };
        }

        void GLFW_RenderSurface::Bind()
        {
            glfwMakeContextCurrent(window_);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void GLFW_RenderSurface::Present()
        {
            glfwSwapBuffers(window_);
        }
    }
} 
