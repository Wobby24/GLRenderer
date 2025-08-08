#include <GLRenderer/Input/GLFW_InputHandler.hpp>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace GLRenderer::Input {

    GLFW_InputHandler::GLFW_InputHandler(GLFWwindow* window)
        : window_(window), scrollX_(0.0), scrollY_(0.0)
    {
        if (!window_) {
            throw std::runtime_error("GLFW_InputHandler requires a valid GLFWwindow pointer");
        }

        // Initialize key states (empty at start)
        previousKeyStates_.clear();
        currentKeyStates_.clear();

        // Register GLFW callbacks
        RegisterCallbacks();
    }

    void GLFW_InputHandler::RegisterCallbacks()
    {
        glfwSetWindowUserPointer(window_, this);
        glfwSetScrollCallback(window_, ScrollCallback);
    }

    void GLFW_InputHandler::Update()
    {
        // Update previous key states
        previousKeyStates_ = currentKeyStates_;

        // Poll current key states for all keys tracked
        for (auto& [key, _] : currentKeyStates_) {
            int state = glfwGetKey(window_, key);
            currentKeyStates_[key] = (state == GLFW_PRESS || state == GLFW_REPEAT);
        }

        // Reset scroll offsets after processing input for this frame
        ResetScroll();
    }

    bool GLFW_InputHandler::IsKeyPressed(int key) const
    {
        // True if key is down now but was not down before
        auto prev = previousKeyStates_.find(key);
        bool wasDown = (prev != previousKeyStates_.end()) ? prev->second : false;

        auto curr = currentKeyStates_.find(key);
        bool isDown = (curr != currentKeyStates_.end()) ? curr->second : (glfwGetKey(window_, key) == GLFW_PRESS);

        return isDown && !wasDown;
    }

    bool GLFW_InputHandler::IsKeyReleased(int key) const
    {
        // True if key was down before but now is not down
        auto prev = previousKeyStates_.find(key);
        bool wasDown = (prev != previousKeyStates_.end()) ? prev->second : false;

        auto curr = currentKeyStates_.find(key);
        bool isDown = (curr != currentKeyStates_.end()) ? curr->second : (glfwGetKey(window_, key) == GLFW_PRESS);

        return !isDown && wasDown;
    }

    bool GLFW_InputHandler::IsKeyDown(int key) const
    {
        // True if key is currently down
        auto curr = currentKeyStates_.find(key);
        if (curr != currentKeyStates_.end()) {
            return curr->second;
        }

        int state = glfwGetKey(window_, key);
        return (state == GLFW_PRESS || state == GLFW_REPEAT);
    }

    void GLFW_InputHandler::GetMousePosition(double& x, double& y) const
    {
        glfwGetCursorPos(window_, &x, &y);
    }

    void GLFW_InputHandler::SetMousePosition(double x, double y)
    {
        glfwSetCursorPos(window_, x, y);
    }

    void GLFW_InputHandler::ResetScroll()
    {
        scrollX_ = 0.0;
        scrollY_ = 0.0;
    }

    // Static callback wrapper to access instance
    void GLFW_InputHandler::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        GLFW_InputHandler* handler = static_cast<GLFW_InputHandler*>(glfwGetWindowUserPointer(window));
        if (handler) {
            handler->scrollX_ += xoffset;
            handler->scrollY_ += yoffset;
        }
    }

} // namespace GLRenderer::Input
