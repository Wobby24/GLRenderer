#pragma once

#include <GLFW/glfw3.h>
#include <GLRenderer/Interface/ISceneInputHandler.hpp>
#include <GLRenderer/OpenGL/Types/GLCameraInputContext.hpp>

namespace GLRenderer {
    class GLSceneInputHandler : public ISceneInputHandler {
    public:
        explicit GLSceneInputHandler(GLCameraInputContext& context);

        void* GetUserPointer() override;

        void RegisterCallbacks(GLFWwindow* window) override;
        void UnregisterCallbacks(GLFWwindow* window) override;

        // Call this once per frame to process movement keys
        void UpdateInput(float deltaTime);

    private:
        GLCameraInputContext& context_;
        GLFWwindow* window_ = nullptr;
    };
}


