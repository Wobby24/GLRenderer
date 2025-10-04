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

        void init();
        void cleanup();

        SceneHandle addScene(std::unique_ptr<IRenderScene> scene) override;
        void initAll() override;
        void cleanupAll() override;

        void updateAll(float deltaTime) override;
        void renderAll() override;
        void resizeAll(int width, int height) override;
        void setWindowToAll(Window::IWindow& window);

        void setCurrentScene(SceneHandle handle) override;
        IRenderScene* getCurrentScene() const override;

        void updateCurrent(float deltaTime) override;
        void renderCurrent() override;
        void initCurrent() override;
        void cleanupCurrent() override;
        void resizeCurrent(int width, int height) override;
        void setWindowToCurrent(Window::IWindow& window);

        // Prevent accidental copying or moving
        GLSceneManager(const GLSceneManager&) = delete;
        GLSceneManager& operator=(const GLSceneManager&) = delete;
        GLSceneManager(GLSceneManager&&) = delete;
        GLSceneManager& operator=(GLSceneManager&&) = delete;

    private:
        std::vector<std::unique_ptr<IRenderScene>> scenes_;
        std::optional<SceneHandle> currentSceneIndex_; // ADD THIS

        bool isCleanedUp_ = false;
        bool isInit_ = false;
        bool allScenesCleanedUp_ = false;
        bool allScenesInit_ = false;
    };

}
