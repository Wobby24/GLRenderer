#pragma once

#include <GLRenderer/Interface/IRenderScene.hpp>
#include <vector>
#include <memory>

namespace GLRenderer {
    class ISceneManager {
    public:
        virtual ~ISceneManager() = default;

        virtual void setScene(std::unique_ptr<IRenderScene> scene) = 0;
        virtual IRenderScene* getCurrentScene() const = 0;

        virtual void updateCurrent(float deltaTime) = 0;
        virtual void renderCurrent() = 0;
        virtual void initCurrent() = 0;
        virtual void cleanupCurrent() = 0;
        virtual void resizeCurrent(int width, int height) = 0;
    };
}


