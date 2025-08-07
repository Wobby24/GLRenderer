#pragma once
#include "Interface/IInputHandler.hpp"
#include <GLFW/glfw3.h>
#include <unordered_map>

namespace GLRenderer::Input {

    class GLFW_InputHandler : public IInputHandler {
    public:
        explicit GLFW_InputHandler(GLFWwindow* window);
        ~GLFW_InputHandler() override = default;

        void Update() override;

        bool IsKeyPressed(int key) const override;
        bool IsKeyReleased(int key) const override;
        bool IsKeyDown(int key) const override;

        void GetMousePosition(double& x, double& y) const override;
        void SetMousePosition(double x, double y) override;

        double GetScrollX() const override { return scrollX_; }
        double GetScrollY() const override { return scrollY_; }
        void ResetScroll() override;

    private:
        GLFWwindow* window_;

        // Scroll offsets
        double scrollX_ = 0.0;
        double scrollY_ = 0.0;

        // Keep track of previous and current key states to detect presses/releases
        std::unordered_map<int, bool> previousKeyStates_;
        std::unordered_map<int, bool> currentKeyStates_;

        static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        // We'll store a pointer to 'this' in GLFW's user pointer to access inside callback
        void RegisterCallbacks();
    };

} // namespace GLRenderer::Input
