#include <GLRenderer/OpenGL/GLSceneManager.hpp>
#include <GLRenderer/Interface/ISceneManager.hpp>
#include <GLRenderer/Window/Interface/IWindow.hpp>
#include <stdexcept>
#include <iostream>

namespace GLRenderer {
    GLSceneManager::GLSceneManager() : isCleanedUp_(false), isInit_(false), allScenesCleanedUp_(false), allScenesInit_(false) {};

    GLSceneManager::~GLSceneManager() {
        cleanupAll();
    }

    void GLSceneManager::init() {
        if (isInit_) return;
        isInit_ = true;
        scenes_.reserve(4);
    }

    void GLSceneManager::cleanup() {
        if (isCleanedUp_) return;
        cleanupAll();
        isCleanedUp_ = true;
    }

    SceneHandle GLSceneManager::addScene(std::unique_ptr<IRenderScene> scene) {
        scenes_.push_back(std::move(scene));
        return scenes_.size() - 1;
    }

    void GLSceneManager::setCurrentScene(SceneHandle handle) {
        if (handle >= scenes_.size()) {
            throw std::out_of_range("Invalid scene handle");
        }
        currentSceneIndex_ = handle;
    }

    IRenderScene* GLSceneManager::getCurrentScene() const {
        if (!currentSceneIndex_.has_value()) {
            return nullptr;
        }
        return scenes_[currentSceneIndex_.value()].get();
    }


    void GLSceneManager::renderCurrent() {
        auto scene = getCurrentScene();
        if (!scene) {
            std::cout << "No current scene set!\n";
            return;
        }
        scene->Render();
    }


    void GLSceneManager::updateCurrent(float deltaTime)
    {
        if (auto scene = getCurrentScene()) {
            scene->Update(deltaTime);
        }
    }

    void GLSceneManager::initCurrent()
    {
        if (auto scene = getCurrentScene()) {
            scene->Init();
        }
    }

    void GLSceneManager::cleanupCurrent()
    {
        if (auto scene = getCurrentScene()) {
            scene->Cleanup();
        }
    }

    void GLSceneManager::resizeCurrent(int width, int height) {
        if (auto scene = getCurrentScene()) {
            scene->OnWindowResize(width, height);  // safe to call on any scene now
        }
    }

    void GLSceneManager::setWindowToCurrent(Window::IWindow& window) {
        if (auto scene = getCurrentScene()) {
            scene->SetWindow(window);
        }
    }

    void GLSceneManager::setWindowToAll(Window::IWindow& window) {
        for (auto& scene : scenes_) {
            scene->SetWindow(window);
        }
    }

    void GLSceneManager::initAll() {
        if (allScenesInit_) return;
        for (auto& scene : scenes_) {
            scene->Init();
        }
        allScenesInit_ = true;
        allScenesCleanedUp_ = false;
    }

    void GLSceneManager::cleanupAll() {
        if (allScenesCleanedUp_) return;
        for (auto& scene : scenes_) {
            scene->Cleanup();
        }
        allScenesCleanedUp_ = true;
    }

    void GLSceneManager::updateAll(float deltaTime) {
        for (auto& scene : scenes_) {
            scene->Update(deltaTime);
        }
    }

    void GLSceneManager::renderAll() {
        for (auto& scene : scenes_) {
            scene->Render();
        }
    }

    void GLSceneManager::resizeAll(int width, int height) {
        for (auto& scene : scenes_) {
            scene->OnWindowResize(width, height);
        }
    }

}
