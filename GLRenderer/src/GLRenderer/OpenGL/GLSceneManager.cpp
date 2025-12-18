#include <GLRenderer/OpenGL/GLSceneManager.hpp>
#include <GLRenderer/Interface/ISceneManager.hpp>
#include <GLRenderer/Window/Interface/IWindow.hpp>
#include <stdexcept>
#include <iostream>

namespace GLRenderer {
    GLSceneManager::GLSceneManager() : window_(nullptr) {
    
    };

    GLSceneManager::~GLSceneManager() {
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

    void GLSceneManager::setWindow(Window::IWindow& window)
    {
        window_ = &window;

        if (currentScene_) {
            currentScene_->SetWindow(window);
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

    void GLSceneManager::setScene(std::unique_ptr<IRenderScene> scene)
    {
        if (currentScene_) {
            currentScene_->Cleanup();
        }

        currentScene_ = std::move(scene);

        if (!window_) {
            throw std::logic_error("Scene set before window assignment");
        }

        currentScene_->SetWindow(*window_);
        currentScene_->Init();
    }

}
