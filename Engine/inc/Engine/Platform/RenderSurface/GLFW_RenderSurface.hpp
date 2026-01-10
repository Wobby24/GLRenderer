#pragma once

#include <stdexcept>
#include <glm/glm.hpp>
#include <Platform/RenderSurface/Interface/IRenderSurface.hpp>

// Forward declare GLFW types
struct GLFWwindow;

namespace Platform {
    namespace RenderSurface {

        class GLFW_RenderSurface final : public IRenderSurface
        {
        public:
            explicit GLFW_RenderSurface(GLFWwindow* window);

            glm::ivec2 GetSize() const override;
            void Bind() override;
            void Present() override;

        private:
            GLFWwindow* window_ = nullptr; // non-owning
        };
    }
} 
