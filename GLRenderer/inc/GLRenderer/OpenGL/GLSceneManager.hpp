#pragma once

#include <GLRenderer/Interface/ISceneManager.hpp>
#include <GLRenderer/Interface/IRenderScene.hpp>
#include <GLRenderer/Window/Interface/IWindow.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <optional>

using SceneHandle = std::size_t; // Define this at the top

namespace GLRenderer {
    class GLSceneManager : public ISceneManager {
    public:
        GLSceneManager();
        ~GLSceneManager() override;

        void setScene(std::unique_ptr<IRenderScene> scene);
        IRenderScene* getCurrentScene() const { return currentScene_.get(); }

        void initCurrent() override;
        void cleanupCurrent() override;
        void renderCurrent() override;
        void updateCurrent(float dt) override;
        void resizeCurrent(int w, int h) override;
        void setWindow(Window::IWindow& window);

        // Prevent accidental copying or moving
        GLSceneManager(const GLSceneManager&) = delete;
        GLSceneManager& operator=(const GLSceneManager&) = delete;
        GLSceneManager(GLSceneManager&&) = delete;
        GLSceneManager& operator=(GLSceneManager&&) = delete;

    private:
        std::unique_ptr<IRenderScene> currentScene_;
        Window::IWindow* window_;
    };

}
