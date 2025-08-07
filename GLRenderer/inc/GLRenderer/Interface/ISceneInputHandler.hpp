#pragma once 

#include <GLFW/glfw3.h>

namespace GLRenderer {
    class ISceneInputHandler{
    public:
    virtual ~ISceneInputHandler() = default;

    virtual void* GetUserPointer() = 0;
    virtual void RegisterCallbacks(GLFWwindow* window) = 0;
    virtual void UnregisterCallbacks(GLFWwindow* window) = 0;
    };
}
