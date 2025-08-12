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
            //imgui
            ImGui_ImplGlfw_CursorPosCallback(win, xpos, ypos);

            // Check if the mouse is locked
            if (glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
                // Cursor is locked, allow app to use input even if ImGui wants it
            }
            else if (ImGui::GetIO().WantCaptureMouse) {
                return; // Let ImGui handle it, don't process input
            }

            auto* ctx = static_cast<GLCameraInputContext*>(glfwGetWindowUserPointer(win));
            if (!ctx || !ctx->camera) return;

            float x = static_cast<float>(xpos);
            float y = static_cast<float>(ypos);

            if (ctx->firstMouse) {
                ctx->lastX = x;
                ctx->lastY = y;
                ctx->firstMouse = false;
            }

            float xoffset = x - ctx->lastX;
            float yoffset = ctx->lastY - y; // Reversed y

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

