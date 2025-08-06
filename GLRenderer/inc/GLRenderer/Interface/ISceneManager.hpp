#pragma once

#include <GLRenderer/Interface/IRenderScene.hpp>
#include <vector>
#include <memory>

using SceneHandle = std::size_t; // Define this at the top

class ISceneManager {
public:
    virtual ~ISceneManager() = default;

    virtual void init() = 0;
    virtual void cleanup() = 0;

    virtual SceneHandle addScene(std::unique_ptr<IRenderScene> scene) = 0;
    virtual void initAll() = 0;
    virtual void cleanupAll() = 0;

    virtual void updateAll(float deltaTime) = 0;
    virtual void renderAll() = 0;
    virtual void resizeAll(int width, int height) = 0;

    virtual void setCurrentScene(SceneHandle handle) = 0;
    virtual IRenderScene* getCurrentScene() const = 0;

    virtual void updateCurrent(float deltaTime) = 0;
    virtual void renderCurrent() = 0;
    virtual void initCurrent() = 0;
    virtual void cleanupCurrent() = 0;
    virtual void resizeCurrent(int width, int height) = 0;
};
