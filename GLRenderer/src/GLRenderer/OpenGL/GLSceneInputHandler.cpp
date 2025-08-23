#include <GLRenderer/OpenGL/Types/GLCameraInputContext.hpp>
#include <GLRenderer/OpenGL/GLSceneInputHandler.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <iostream>

namespace GLRenderer {
    GLSceneInputHandler::GLSceneInputHandler(GLCameraInputContext& context)
        : context_(context), window_(nullptr) {
    }

    void* GLSceneInputHandler::GetUserPointer() {
        return &context_;
    }

    void GLSceneInputHandler::RegisterCallbacks(GLFWwindow* window) {
        window_ = window;
        glfwSetWindowUserPointer(window_, &context_);

        // --- REQUIRED FOR IMGUI TEXT INPUT ---
        glfwSetCharCallback(window_, ImGui_ImplGlfw_CharCallback);

        // Mouse movement
        glfwSetCursorPosCallback(window_, [](GLFWwindow* win, double xpos, double ypos) {
            ImGui_ImplGlfw_CursorPosCallback(win, xpos, ypos);

            if (glfwGetInputMode(win, GLFW_CURSOR) != GLFW_CURSOR_DISABLED &&
                ImGui::GetIO().WantCaptureMouse) {
                return;
            }

            auto* ctx = static_cast<GLCameraInputContext*>(glfwGetWindowUserPointer(win));
            if (!ctx || !ctx->camera) return;

            if (ctx->ignoreNextMouseDelta) {
                ctx->ignoreNextMouseDelta = false;
                return;
            }

            float x = static_cast<float>(xpos);
            float y = static_cast<float>(ypos);

            if (ctx->firstMouse) {
                // This event is likely from the old mouse position (before locking)
                ctx->firstMouse = false;

                // Force mouse to center again, in case it hasn't yet
                int width, height;
                glfwGetWindowSize(win, &width, &height);
                glfwSetCursorPos(win, width / 2.0, height / 2.0);
                ctx->lastX = width / 2.0f;
                ctx->lastY = height / 2.0f;

                return;
            }

            float xoffset = x - ctx->lastX;
            float yoffset = ctx->lastY - y; // Reversed

            ctx->lastX = x;
            ctx->lastY = y;

            ctx->camera->processMouseMovement(xoffset, yoffset);
            });

        // Scroll wheel
        glfwSetScrollCallback(window_, [](GLFWwindow* win, double xoffset, double yoffset) {
            //imgui callback 
            ImGui_ImplGlfw_ScrollCallback(win, xoffset, yoffset);

            if (glfwGetInputMode(win, GLFW_CURSOR) != GLFW_CURSOR_DISABLED &&
                ImGui::GetIO().WantCaptureMouse) {
                return;
            }

            (void)xoffset;
            auto* ctx = static_cast<GLCameraInputContext*>(glfwGetWindowUserPointer(win));
            if (!ctx || !ctx->camera) return;

            ctx->camera->processMouseScroll(static_cast<float>(yoffset));
            });

        // Mouse button
        glfwSetMouseButtonCallback(window_, [](GLFWwindow* win, int button, int action, int mods) {
            ImGui_ImplGlfw_MouseButtonCallback(win, button, action, mods);

            if (glfwGetInputMode(win, GLFW_CURSOR) != GLFW_CURSOR_DISABLED &&
                ImGui::GetIO().WantCaptureMouse) {
                return;
            }
            
            auto* ctx = static_cast<GLCameraInputContext*>(glfwGetWindowUserPointer(win));
            if (!ctx || !ctx->camera) return;
             if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                // Do something on left click
             }
            });

        // Keyboard input
        glfwSetKeyCallback(window_, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
            ImGui_ImplGlfw_KeyCallback(win, key, scancode, action, mods);

            if (ImGui::GetIO().WantCaptureKeyboard) return;

            // Custom key event handling example:
             auto* ctx = static_cast<GLCameraInputContext*>(glfwGetWindowUserPointer(win));
             if (!ctx || !ctx->camera) return;
             if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                 // Maybe exit or toggle something
             }
            });
    }

    void GLSceneInputHandler::UpdateCursorMode() {
        if (!window_) return;

        int currentMode = glfwGetInputMode(window_, GLFW_CURSOR);
        if (currentMode != lastCursorMode_) {
            lastCursorMode_ = currentMode;

            int width, height;
            glfwGetWindowSize(window_, &width, &height);

            if (currentMode == GLFW_CURSOR_DISABLED) {
                // Cursor just got locked — reset mouse input state to center
                context_.ignoreNextMouseDelta = true;
                glfwSetCursorPos(window_, width / 2.0, height / 2.0);
                context_.lastX = width / 2.0f;
                context_.lastY = height / 2.0f;
            }
            else if (currentMode == GLFW_CURSOR_NORMAL) {
                // Cursor just got unlocked — reset mouse input state to current cursor pos
                double xpos, ypos;
                glfwGetCursorPos(window_, &xpos, &ypos);

                context_.ignoreNextMouseDelta = true;
                context_.lastX = static_cast<float>(xpos);
                context_.lastY = static_cast<float>(ypos);
            }
        }
    }

    void GLSceneInputHandler::UnregisterCallbacks(GLFWwindow* window) {
        if (window == window_) {
            glfwSetCursorPosCallback(window_, nullptr);
            glfwSetScrollCallback(window_, nullptr);
            context_.firstMouse = true;
            window_ = nullptr;
        }
    }

    void GLSceneInputHandler::UpdateInput(float deltaTime) {
        if (!window_ || !context_.camera) return;

        if (ImGui::GetIO().WantCaptureKeyboard) return;

        UpdateCursorMode();

        if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
            context_.camera->processKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) 
            context_.camera->processKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
            context_.camera->processKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
            context_.camera->processKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS)
            context_.camera->processKeyboard(UP, deltaTime);
        if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS)
            context_.camera->processKeyboard(DOWN, deltaTime);
    }

}

