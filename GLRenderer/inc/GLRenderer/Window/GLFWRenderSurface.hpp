#pragma once

#include <stdexcept>
#include <glm/glm.hpp>
#include <GLRenderer/Window/Interface/IRenderSurface.hpp>

// Forward declare GLFW types
struct GLFWwindow;

namespace GLRenderer {

    class GLFWRenderSurface final : public IRenderSurface
    {
    public:
        explicit GLFWRenderSurface(GLFWwindow* window);

        glm::ivec2 GetSize() const override;
        void Bind() override;
        void Present() override;

    private:
        GLFWwindow* window_ = nullptr; // non-owning
    };

} // namespace GLRenderer
